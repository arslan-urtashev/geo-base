// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "osm_parser.h"

#include <geo_base/util/exception.h>
#include <geo_base/util/log.h>
#include <zlib.h>

namespace geo_base {
namespace tool {

static void unpack(char const *ptr, size_t count, dynarray_t<char> *buffer)
{
    unsigned long reply_count = buffer->size();
    if (uncompress((unsigned char*) buffer->data(), &reply_count, (unsigned char *) ptr, count) != Z_OK)
        throw exception_t("Unable uncompress zlib data");
}

static bool parse_primitive_block(osmpbf::blob_t const &blob, osmpbf::primitive_block_t *block,
	allocator_t *allocator)
{
	dynarray_t<char> buffer(blob.raw_size(), blob.raw_size(), allocator);

	auto const &z = blob.zlib_data();
	unpack(z.data(), z.size(), &buffer);

	if (!block->ParseFromArray(buffer.data(), buffer.size()))
		return false;

	return true;
}

template<typename type_t>
static dynarray_t<osm_tag_t> make_tags(type_t const &obj, osmpbf::primitive_block_t const &block,
	allocator_t *allocator)
{
	dynarray_t<osm_tag_t> tags(obj.keys_size(), allocator);
	for (int i = 0; i < obj.keys_size(); ++i) {
		size_t k = obj.keys(i);
		size_t v = obj.vals(i);

		osm_tag_t tag = {
			block.string_table().s(k).c_str(),
			block.string_table().s(v).c_str()
		};

		tags.push_back(tag);
	}
	return tags;
}

void osm_parser_t::process_primitive_groups(osmpbf::primitive_block_t const &block)
{
	for (int k = 0; k < block.primitive_group_size(); ++k) {
		osmpbf::primitive_group_t const &group = block.primitive_group(k);

		for (int i = 0; i < group.nodes_size(); ++i) {
			osmpbf::node_t const &node = group.nodes(i);

			osm_location_t location;
			location.lat = 1e-9 * (block.lat_offset() + block.granularity() * node.lat());
			location.lon = 1e-9 * (block.lon_offset() + block.granularity() * node.lon());

			process_node(node.id(), location, make_tags(node, block, allocator_));
		}

		if (group.has_dense()) {
			osmpbf::dense_nodes_t const &nodes = group.dense();

			osm_id_t osm_id = 0;
			osm_location_t location = { 0, 0 };
			int current_tag = 0;

			for (int i = 0; i < nodes.id_size(); ++i) {
				osm_id += nodes.id(i);
				location.lon += 1e-9 * (block.lon_offset() + block.granularity() * nodes.lon(i));
				location.lat += 1e-9 * (block.lat_offset() + block.granularity() * nodes.lat(i));
				
				dynarray_t<osm_tag_t> tags(nodes.keys_vals_size(), allocator_);

				while (current_tag < nodes.keys_vals_size() && nodes.keys_vals(current_tag) != 0) {
					size_t k = nodes.keys_vals(current_tag);
					size_t v = nodes.keys_vals(current_tag + 1);

					osm_tag_t tag = {
						block.string_table().s(k).c_str(),
						block.string_table().s(v).c_str()
					};
					tags.push_back(tag);

					current_tag += 2;
				}
				++current_tag;

				process_node(osm_id, location, tags);
			}
		}

		for (int i = 0; i < group.ways_size(); ++i) {
			osmpbf::way_t const &w = group.ways(i);

			osm_id_t ref_id = 0;
			dynarray_t<osm_id_t> refs(w.refs_size(), allocator_);
			for (int j = 0; j < w.refs_size(); ++j) {
				ref_id += w.refs(j);
				refs.push_back(ref_id);
			}

			process_way(w.id(), make_tags(w, block, allocator_), refs);
		}

		for (int i = 0; i < group.relations_size(); ++i) {
			osmpbf::relation_t const &r = group.relations(i);

			osm_id_t ref_id = 0;
			dynarray_t<osm_reference_t> refs(r.memids_size(), allocator_);
			for (int j = 0; j < r.memids_size(); ++j) {
				ref_id += r.memids(j);
				osm_reference_t reference = {
					r.types(j),
					ref_id,
					block.string_table().s(r.roles_sid(j)).c_str()
				};
				refs.push_back(reference);
			}

			process_relation(r.id(), make_tags(r, block, allocator_), refs);
		}
	}
}

void osm_parser_t::parse(osm_reader_t *reader)
{
	osmpbf::blob_header_t header;
	osmpbf::blob_t blob;
	osmpbf::primitive_block_t block;

	while (reader->read(&header, &blob, allocator_)) {
		if (header.type() == "OSMHeader")
			continue;

		if (header.type() != "OSMData")
			continue;

		if (blob.has_raw())
			throw exception_t("Can't parse raw data!");

		if (blob.has_lzma_data())
			throw exception_t("Can't parse lzma data!");

		if (!parse_primitive_block(blob, &block, allocator_))
			throw exception_t("Can't parse primitive block!");

		process_primitive_groups(block);
	}
}

} // namespace tool
} // namespace geo_base
