// Copyright (c) 2016 Urtashev Arslan. All rights reserved.
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

#include <zlib.h>

#include <google/protobuf/arena.h>

#include <geo_base/open_street_map/parser.h>
#include <geo_base/util/dynarray.h>
#include <geo_base/util/file.h>
#include <geo_base/util/file_stream.h>

namespace geo_base {
namespace open_street_map {

#define CATCH_RUN(type, function) \
	try { \
		function; \
	} catch (std::exception const &e) { \
		log_error("Exception thrown while processing %s of %s: %s", \
			type, find_name(kvs), e.what()); \
		throw; \
	} catch (...) { \
		log_error("Unknown exception thrown while processing %s of %s", \
			type, find_name(kvs)); \
		throw; \
	}

static void unpack(char const *ptr, size_t count, dynarray_t<char> *buffer)
{
	unsigned long size = buffer->size();
	int ret = uncompress((unsigned char *) buffer->data(), &size, (unsigned char *) ptr, count);

	if (ret != Z_OK)
		throw exception_t("unpack error: %d", ret);

	buffer->resize(size);
}

static bool parse_basic_block(proto::blob_t const &blob, proto::basic_block_t *block,
	allocator_t *allocator)
{
	dynarray_t<char> buffer(blob.raw_size(), blob.raw_size(), allocator);

	auto const &z = blob.zlib_data();
	unpack(z.data(), z.size(), &buffer);

	if (!block->ParseFromArray(buffer.data(), buffer.size()))
		return false;

	return true;
}

static double normalize_coordinate(double x)
{
	return x * 1e-9;
}

static location_t make_location(proto::basic_block_t const &block, double lat, double lon)
{
	location_t l;
	l.lat = normalize_coordinate(block.lat_off() + block.granularity() * lat);
	l.lon = normalize_coordinate(block.lon_off() + block.granularity() * lon);
	return l;
}

template<typename type_t>
static kvs_t make_kvs(type_t const &obj, proto::basic_block_t const &block, allocator_t *allocator)
{
	kvs_t kvs(obj.keys_size(), allocator);

	for (int i = 0; i < obj.keys_size(); ++i) {
		size_t const k = obj.keys(i);
		size_t const v = obj.vals(i);
		kvs.push_back(kv_t{block.string_table().s(k).c_str(), block.string_table().s(v).c_str()});
	}

	return kvs;
}

static kvs_t make_kvs(int *offset, proto::dense_nodes_t const &nodes,
	proto::basic_block_t const &block, allocator_t *allocator)
{
	kvs_t tags(nodes.kvs_size(), allocator);

	while (*offset < nodes.kvs_size() && nodes.kvs(*offset) != 0) {
		size_t const k = nodes.kvs(*offset);
		size_t const v = nodes.kvs(*offset + 1);
		tags.push_back(kv_t{block.string_table().s(k).c_str(), block.string_table().s(v).c_str()});
		*offset += 2;
	}

	*offset += 1;

	return tags;
}

static location_t &operator += (location_t &a, location_t const &b)
{
	a.lat += b.lat;
	a.lon += b.lon;
	return a;
}

void parser_t::process_dense_nodes(proto::dense_nodes_t const &nodes,
	proto::basic_block_t const &block)
{
	geo_id_t geo_id = 0;
	location_t location(0, 0);
	int tags_offset = 0;
	for (int i = 0; i < nodes.id_size(); ++i) {
		geo_id += nodes.id(i);
		location += make_location(block, nodes.lat(i), nodes.lon(i));
		kvs_t const kvs = make_kvs(&tags_offset, nodes, block, allocator_);
		CATCH_RUN("nodes", process_node(geo_id, location, kvs));
		++dense_nodes_processed_;
	}
}

void parser_t::process_basic_groups(proto::basic_block_t const &block)
{
	for (int k = 0; k < block.basic_groups_size(); ++k) {
		proto::basic_group_t const &group = block.basic_groups(k);

		if (!(processing_disabled_mask_ & NODE_PROC_DISABLED)) {
			for (int i = 0; i < group.nodes_size(); ++i) {
				proto::node_t const &node = group.nodes(i);
				location_t const location = make_location(block, node.lat(), node.lon());
				kvs_t const kvs = make_kvs(node, block, allocator_);
				CATCH_RUN("nodes", process_node(node.id(), location, kvs));
				++nodes_processed_;
			}

			if (group.has_dense_nodes()) {
				proto::dense_nodes_t const &nodes = group.dense_nodes();
				process_dense_nodes(nodes, block);
			}
		}

		if (!(processing_disabled_mask_ & WAY_PROC_DISABLED)) {
			for (int i = 0; i < group.ways_size(); ++i) {
				proto::way_t const &w = group.ways(i);

				geo_ids_t references(w.refs_size(), w.refs_size(), allocator_);

				for (int j = 0; j < w.refs_size(); ++j)
					references[j] = w.refs(j);
				for (int j = 0; j < w.refs_size() - 1; ++j)
					references[j + 1] += references[j];

				kvs_t const kvs = make_kvs(w, block, allocator_);
				CATCH_RUN("ways", process_way(w.id(), kvs, references));

				++ways_processed_;
			}
		}

		if (!(processing_disabled_mask_ & RELATION_PROC_DISABLED)) {
			for (int i = 0; i < group.relations_size(); ++i) {
				proto::relation_t const &r = group.relations(i);

				references_t references(r.member_ids_size(), r.member_ids_size(), allocator_);

				for (int j = 0; j < r.member_ids_size(); ++j)
					references[j].geo_id = r.member_ids(j);
				for (int j = 0; j < r.member_ids_size(); ++j)
					references[j].type = (reference_t::type_t) r.member_types(j);
				for (int j = 0; j < r.member_ids_size(); ++j)
					references[j].role = block.string_table().s(r.roles_sid(j)).c_str();
				for (int j = 0; j < r.member_ids_size() - 1; ++j)
					references[j + 1].geo_id += references[j].geo_id;

				kvs_t const kvs = make_kvs(r, block, allocator_);
				CATCH_RUN("relations", process_relation(r.id(), kvs, references));

				++relations_processed_;
			}
		}
	}
}

template<typename message_t>
message_t *create_message(google::protobuf::Arena *arena)
{
	return google::protobuf::Arena::CreateMessage<message_t>(arena);
}

void parser_t::parse(reader_t *reader)
{
	google::protobuf::ArenaOptions options;
	
	std::function<void *(size_t)> block_alloc = [&] (size_t n) {
		return allocator_->allocate(n);
	};
	std::function<void (void *, size_t)> block_dealloc = [&] (void *ptr, size_t n) {
		allocator_->deallocate(ptr, n);
	};

	options.block_alloc = block_alloc.target<void *(size_t)>();
	options.block_dealloc = block_dealloc.target<void (void *, size_t)>();

	google::protobuf::Arena arena(options);

	while (true) {
		proto::blob_header_t *header = create_message<proto::blob_header_t>(&arena);
		proto::blob_t *blob = create_message<proto::blob_t>(&arena);
		proto::basic_block_t *block = create_message<proto::basic_block_t>(&arena);

		if (!reader->read(header, blob, allocator_))
			break;

		if (header->type() == "OSMHeader")
			continue;

		if (header->type() != "OSMData")
			continue;

		if (blob->has_raw())
			throw exception_t("Unable parse raw data");

		if (blob->has_lzma_data())
			throw exception_t("Unable parse lzma data");

		if (!parse_basic_block(*blob, block, allocator_))
			throw exception_t("Unable parse block");

		process_basic_groups(*block);

		++blocks_processed_;

		arena.Reset();
	}
}

size_t optimal_threads_number()
{
	size_t threads_count = std::thread::hardware_concurrency();
	if (threads_count == 0)
		++threads_count;
	return std::min(threads_count, 12ul);
}

} // namespace open_street_map
} // namespace geo_base
