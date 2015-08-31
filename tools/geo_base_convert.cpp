#include <stdint.h>
#include <netinet/in.h>

#include <thread>
#include <mutex>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <osmpbf/osmpbf.h>

#include "fd_base.hpp"
#include "location.hpp"
#include "log.hpp"
#include "zlib.hpp"
#include "stop_watch.hpp"

using namespace geo_base;

static size_t const DEFAULT_THREADS_COUNT = 2;

typedef region_id_t osm_id_t;

typedef OSMPBF::BlobHeader blob_header_t;
typedef OSMPBF::Blob blob_t;
typedef OSMPBF::PrimitiveBlock block_t;
typedef OSMPBF::PrimitiveGroup group_t;
typedef OSMPBF::Node node_t;
typedef OSMPBF::DenseNodes dense_nodes_t;
typedef OSMPBF::Way way_t;
typedef OSMPBF::Relation relation_t;

struct kv_t {
	char const *k;
	char const *v;
};

enum class member_type_t {
	unknown = -1,
	way
};

struct reference_t {
	member_type_t member_type;
	osm_id_t osm_id;
	char const *role;
};

static reference_t make_reference(OSMPBF::Relation::MemberType const &member_type, osm_id_t osm_id, std::string const &role)
{
	reference_t ref;
	
	ref.member_type = member_type_t::unknown;
	if (member_type == OSMPBF::Relation::WAY)
		ref.member_type = member_type_t::way;
	ref.osm_id = osm_id;
	ref.role = role.c_str();
	
	return ref;
}

class pbf_reader_t {
public:
	pbf_reader_t(char const *path)
	{
		fd_base.rd(path);
	}

	bool read(blob_header_t *header, blob_t *blob, std::vector<char> *buf) const
	{
		std::lock_guard<std::mutex> lock(mutex);
		
		int32_t header_size = 0;
		if (!read((char *) &header_size, sizeof(header_size)))
			return false;

		header_size = ntohl(header_size);

		buf->resize(header_size);
		if (!read(buf->data(), buf->size())) {
			log_error("geo-base-convert") << "Can't read header";
			return false;
		}

		if (!header->ParseFromArray(buf->data(), buf->size())) {
			log_error("geo-base-convert") << "Can't parse header";
			return false;
		}

		int32_t blob_size = header->datasize();

		buf->resize(blob_size);
		if (!read(buf->data(), buf->size())) {
			log_error("geo-base-convert") << "Can't read blob";
			return false;
		}

		if (!blob->ParseFromArray(buf->data(), buf->size())) {
			log_error("geo-base-convert") << "Can't parse blob";
			return false;
		}

		return true;
	}

private:
	bool read(char *ptr, size_t count) const
	{
		while (count > 0) {
			ssize_t ret = ::read(fd_base.fd(), ptr, count);
			if (ret < 0)
				throw exception_t("Can't read: %s", strerror(errno));
			if (ret == 0)
				return false;
			ptr += ret;
			count -= ret;
		}
		return true;
	}

	fd_base_t fd_base;
	mutable std::mutex mutex;
};

template<typename callback_t>
class pbf_parser_t {
public:
	pbf_parser_t(pbf_reader_t const &reader, callback_t &callback)
		: reader(&reader)
		, callback(callback)
	{
	}

	void operator () ()
	{
		blob_header_t header;
		blob_t blob;
		block_t block;
		group_t group;

		read_count = 0;
		while (reader->read(&header, &blob, &buffer)) {
			if (header.type() == "OSMHeader")
				continue;

			if (header.type() != "OSMData")
				throw exception_t("Unknown header type: %s", header.type().c_str());

			if (blob.has_raw())
				throw exception_t("%s", "Can't parse raw data");

			if (blob.has_lzma_data())
				throw exception_t("%s", "Can't parse lzma data");

			if (!parse_block(blob, &block))
				throw exception_t("%s", "Can't parse block");

			if (!process_groups(block))
				throw exception_t("%s", "Can't process group");

			++read_count;
		}
	}

	size_t count() const
	{
		return read_count;
	}

private:
	template<typename x_t>
	std::vector<kv_t> const &get_tags(x_t const &obj, block_t const &block)
	{
		kvs.clear();
		for (size_t i = 0; i < obj.keys_size(); ++i) {
			uint64_t k = obj.keys(i);
			uint64_t v = obj.vals(i);

			kv_t kv;
			kv.k = block.stringtable().s(k).c_str();
			kv.v = block.stringtable().s(v).c_str();

			kvs.push_back(kv);
		}
		return kvs;
	}

	bool parse_block(blob_t const &blob, block_t *block)
	{
		buffer.resize(blob.raw_size());

		zlib_t zlib;
		auto const &z = blob.zlib_data();
		size_t unpack_size = zlib.unpack(z.data(), z.size(), buffer.data(), buffer.size());

		buffer.resize(unpack_size);

		if (!block->ParseFromArray(buffer.data(), buffer.size()))
			return false;

		return true;
	}

	bool process_groups(block_t const &block)
	{
		for (size_t i = 0; i < block.primitivegroup_size(); ++i) {
			group_t const &group = block.primitivegroup(i);

			for (size_t i = 0; i < group.nodes_size(); ++i) {
				node_t const &node = group.nodes(i);

				location_t l;
				l.lon = 1e-9 * (block.lon_offset() + block.granularity() * node.lon());
				l.lat = 1e-9 * (block.lat_offset() + block.granularity() * node.lat());

				callback.node_callback(node.id(), l, get_tags(node, block));
			}

			if (group.has_dense()) {
				dense_nodes_t const &nodes = group.dense();

				osm_id_t osm_id = 0;
				location_t location;

				size_t cur_kv = 0;

				for (size_t i = 0; i < nodes.id_size(); ++i) {
					osm_id += nodes.id(i);
					location.lon += 1e-9 * (block.lon_offset() + block.granularity() * nodes.lon(i));
					location.lat += 1e-9 * (block.lat_offset() + block.granularity() * nodes.lat(i));

					kvs.clear();
					while (cur_kv < nodes.keys_vals_size() && nodes.keys_vals(cur_kv) != 0) {
						uint64_t k = nodes.keys_vals(cur_kv);
						uint64_t v = nodes.keys_vals(cur_kv + 1);

						kv_t kv;
						kv.k = block.stringtable().s(k).c_str();
						kv.v = block.stringtable().s(v).c_str();

						kvs.push_back(kv);

						cur_kv += 2;
					}
					++cur_kv;

					callback.node_callback(osm_id, location, kvs);
				}
			}

			for (size_t i = 0; i < group.ways_size(); ++i) {
				way_t const &w = group.ways(i);
				
				way_refs.clear();
				osm_id_t ref = 0;
				for (size_t j = 0; j < w.refs_size(); ++j) {
					ref += w.refs(j);
					way_refs.push_back(ref);
				}

				callback.way_callback(w.id(), get_tags(w, block), way_refs);
			}

			for (size_t i = 0; i < group.relations_size(); ++i) {
				relation_t const &r = group.relations(i);

				refs.clear();
				osm_id_t osm_id = 0;
				for (size_t j = 0; j < r.memids_size(); ++j) {
					osm_id += r.memids(j);
					refs.push_back(make_reference(r.types(j), osm_id, block.stringtable().s(r.roles_sid(j))));
				}

				callback.relation_callback(r.id(), get_tags(r, block), refs);
			}
		}

		return true;
	}

	pbf_reader_t const *reader;
	callback_t &callback;
	size_t read_count;
	std::vector<char> buffer;
	std::vector<kv_t> kvs;
	std::vector<osm_id_t> way_refs;
	std::vector<reference_t> refs;
};

template<typename callback_t>
class pbf_mt_parser_t {
public:
	pbf_mt_parser_t(char const *path, std::vector<callback_t> &callbacks)
		: reader(path)
		, pbf_path(path)
		, callbacks(callbacks)
	{
	}

	void operator () ()
	{
		log_info("geo-base-convert") << "Parse \"" << pbf_path << "\"...";
		stop_watch_t stop_watch;

		std::vector<pbf_parser_t<callback_t>> parsers;
		for (size_t i = 0; i < callbacks.size(); ++i)
			parsers.emplace_back(reader, callbacks[i]);

		std::vector<std::thread> threads(parsers.size());
		for (size_t i = 0; i < threads.size(); ++i)
			threads[i] = std::thread(std::ref(parsers[i]));

		for (size_t i = 0; i < threads.size(); ++i)
			threads[i].join();

		log_info("geo-base-convert") << "Parsed \"" << pbf_path << "\" by " << stop_watch.checkpoint() / 60. << " MIN";
	}

private:
	pbf_reader_t reader;
	std::string pbf_path;
	std::vector<callback_t> &callbacks;
};

class pbf_callback_t {
public:
	void node_callback(osm_id_t, location_t const &, std::vector<kv_t> const &)
	{
	}

	void way_callback(osm_id_t, std::vector<kv_t> const &, std::vector<osm_id_t> const &)
	{
	}

	void relation_callback(osm_id_t, std::vector<kv_t> const &, std::vector<reference_t> const &)
	{
	}
};

static bool is_boundary(std::vector<kv_t> const &kvs)
{
	bool ok = false;
	for (kv_t const &kv : kvs)
		if (
			!strcmp("admin_level", kv.k)
			|| (
				!strcmp(kv.k, "boundary")
				&& !strcmp(kv.v, "administrative")
			)
			|| (
				!strcmp(kv.k, "place")
				&& (
					!strcmp(kv.v, "city")
					|| !strcmp(kv.v, "country")
					|| !strcmp(kv.v, "state")
					|| !strcmp(kv.v, "region")
					|| !strcmp(kv.v, "province")
					|| !strcmp(kv.v, "district")
					|| !strcmp(kv.v, "municipality")
					|| !strcmp(kv.v, "suburb")
					|| !strcmp(kv.v, "quarter")
					|| !strcmp(kv.v, "neighbourhood")
					|| !strcmp(kv.v, "city_block")
					|| !strcmp(kv.v, "town")
					|| !strcmp(kv.v, "village")
				)
			)
		) {
			ok = true;
			break;
		}
	if (!ok)
		return false;
	ok = false;
	for (kv_t const &kv : kvs) {
		if (!strcmp(kv.k, "name")) {
			ok = true;
			break;
		}
	}
	return ok;
}

static bool is_way_ref(reference_t const &r)
{
	if (r.member_type == member_type_t::way && (!strcmp(r.role, "outer") || !r.role))
		return true;
	return false;
}

struct ways_callback_t : public pbf_callback_t {
	std::unordered_set<osm_id_t> ways;

	void way_callback(osm_id_t osm_id, std::vector<kv_t> const &kvs, std::vector<osm_id_t> const &refs)
	{
		if (is_boundary(kvs) && refs.front() == refs.back())
			ways.insert(osm_id);
	}

	void relation_callback(osm_id_t , std::vector<kv_t> const &kvs, std::vector<reference_t> const &refs)
	{
		if (is_boundary(kvs))
			for (reference_t const &r : refs)
				if (is_way_ref(r))
					ways.insert(r.osm_id);
	}
};

struct node_ids_callback_t : public pbf_callback_t {
	std::unordered_set<osm_id_t> const &need_ways;

	std::unordered_set<osm_id_t> nodes;
	std::unordered_map<osm_id_t, std::vector<osm_id_t>> ways;

	node_ids_callback_t(std::unordered_set<osm_id_t> const &w)
		: need_ways(w)
	{
	}

	void way_callback(osm_id_t osm_id, std::vector<kv_t> const &, std::vector<osm_id_t> const &n)
	{
		if (need_ways.find(osm_id) != need_ways.end())
			for (osm_id_t node_id : n) {
				nodes.insert(node_id);
				ways[osm_id].push_back(node_id);
			}
	}
};

struct nodes_callback_t : public pbf_callback_t {
	std::unordered_set<osm_id_t> const &need_nodes;

	std::unordered_map<osm_id_t, location_t> nodes;

	nodes_callback_t(std::unordered_set<osm_id_t> const &need_nodes)
		: need_nodes(need_nodes)
	{
	}

	void node_callback(osm_id_t osm_id, location_t const &location, std::vector<kv_t> const &)
	{
		if (need_nodes.find(osm_id) != need_nodes.end())
			nodes[osm_id] = location;
	}
};

struct parser_t : public pbf_callback_t {
	std::unordered_map<osm_id_t, location_t> const &nodes;
	std::unordered_map<osm_id_t, std::vector<osm_id_t>> const &ways;

	parser_t(std::unordered_map<osm_id_t> const &nodes, std::unordered_map<osm_id_t, std::vector<osm_id_t>> const &ways)
		: nodes(nodes)
		, ways(ways)
	{
	}
};

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	
	log_level(log_level_t::debug);

	if (argc < 2) {
		log_error("geo-base-convert") << "geo-base-convert <OSM.pbf>";
		return -1;
	}

	size_t threads_count = DEFAULT_THREADS_COUNT;

	std::vector<ways_callback_t> ways_callbacks(threads_count);
	pbf_mt_parser_t<ways_callback_t>(argv[1], ways_callbacks)();

	std::unordered_set<osm_id_t> ways;
	for (ways_callback_t &w : ways_callbacks) {
		ways.insert(w.ways.begin(), w.ways.end());
		w.ways.clear();
	}

	log_info("geo-base-convert", "ways_callback_t") << ways.size();

	std::vector<node_ids_callback_t> node_ids_callback;
	for (size_t i = 0; i < threads_count; ++i)
		node_ids_callback.emplace_back(ways);

	pbf_mt_parser_t<node_ids_callback_t>(argv[1], node_ids_callback)();

	std::unordered_set<osm_id_t> node_ids;
	for (node_ids_callback_t &n : node_ids_callback) {
		node_ids.insert(n.nodes.begin(), n.nodes.end());
		n.nodes.clear();
	}

	std::unordered_map<osm_id_t, std::vector<osm_id_t>> way_nodes;
	for (node_ids_callback_t &n : node_ids_callback) {
		way_nodes.insert(n.ways.begin(), n.ways.end());
		n.ways.clear();
	}

	log_info("geo-base-convert", "node_ids_callback_t") << node_ids.size();

	std::vector<nodes_callback_t> nodes_callback;
	for (size_t i = 0; i < threads_count; ++i)
		nodes_callback.emplace_back(node_ids);

	pbf_mt_parser_t<nodes_callback_t>(argv[1], nodes_callback)();

	std::unordered_map<osm_id_t, location_t> nodes;
	for (nodes_callback_t &n : nodes_callback) {
		nodes.insert(n.nodes.begin(), n.nodes.end());
		n.nodes.clear();
	}

	log_info("geo-base-convert", "nodes_callback_t") << nodes.size();

	return 0;
}
