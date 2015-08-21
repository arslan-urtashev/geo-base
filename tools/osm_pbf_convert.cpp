#include "location.h"
#include "log.h"
#include "typedef.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "vector.h"

#include "osmpbfreader/osmpbfreader.h"

#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>

using namespace troll;
using namespace CanalTP;

using osm_id_t = uint64_t;

unordered_set_t<osm_id_t> debug_osm_ids;
unordered_set_t<osm_id_t> need_ways;
unordered_set_t<osm_id_t> need_nodes;
unordered_map_t<osm_id_t, location_t> nodes;
unordered_map_t<osm_id_t, vector_t<osm_id_t>> ways;

static bool is_boundary(Tags const &tags)
{
	for (auto const &p : tags)
		if (
			p.first == "admin_level"
			|| (
				p.first == "boundary"
				&& p.second == "administrative"
			)
			|| (
				p.first == "place"
				&& (
					p.second == "city"
					|| p.second == "country"
					|| p.second == "state"
					|| p.second == "region"
					|| p.second == "province"
					|| p.second == "district"
					|| p.second == "municipality"
					|| p.second == "suburb"
					|| p.second == "quarter"
					|| p.second == "neighbourhood"
					|| p.second == "city_block"
					|| p.second == "town"
					|| p.second == "village"
				)
			)
		)
			return true;
	return false;
}

static bool is_way_ref(Reference const &r)
{
	if (
		r.member_type == OSMPBF::Relation::WAY
		&& (
			r.role == "outer"
			|| r.role.empty()
		)
	)
		return true;
	return false;
}

struct need_ways_visit_t {
	void node_callback(osm_id_t, double , double , const Tags &)
	{
	}

	void way_callback(osm_id_t, Tags const &, std::vector<osm_id_t> const &)
	{
	}

	void relation_callback(osm_id_t osm_id, Tags const &tags, References const &refs)
	{
		bool boundary = is_boundary(tags);

		if (boundary) {
			for (Reference const &r : refs) {
				if (is_way_ref(r))
					need_ways.insert(r.member_id);
			}
		}

		if (debug_osm_ids.find(osm_id) != debug_osm_ids.end()) {
			for (auto const &tag : tags)
				log_info(osm_id) << tag.first << " = " << tag.second;
			if (!boundary)
				log_error(osm_id) << "Not a boundary!";
		}
	}
};

struct need_nodes_visit_t {
	void node_callback(osm_id_t, double , double , const Tags &)
	{
	}

	void way_callback(osm_id_t osm_id, Tags const &, std::vector<osm_id_t> const &nodes)
	{
		if (need_ways.find(osm_id) != need_ways.end())
			for (osm_id_t node_id : nodes) {
				need_nodes.insert(node_id);
				ways[osm_id].push_back(node_id);
			}
	}

	void relation_callback(osm_id_t , Tags const &, References const &)
	{
	}
};

struct parser_t {
	vector_t<location_t> locations;

	unordered_map_t<osm_id_t, vector_t<osm_id_t>> graph;
	unordered_set_t<osm_id_t> used;

	void node_callback(osm_id_t osm_id, double lon, double lat, const Tags &)
	{
		if (need_nodes.find(osm_id) != need_nodes.end())
			nodes[osm_id] = location_t(lon, lat);
	}

	void way_callback(osm_id_t, Tags const &, std::vector<osm_id_t> const &)
	{
	}

	void save_locations(osm_id_t osm_id, bool reversed = false)
	{
		used.insert(osm_id);

		if (ways[osm_id].empty())
			return;

		count_t locations_count = locations.size();
		for (osm_id_t node_id : ways[osm_id])
			locations.push_back(nodes[node_id]);
		if (reversed)
			reverse(locations.begin() + locations_count, locations.end());

		location_t location = locations.back();

		osm_id_t node_id = (reversed ? ways[osm_id].front() : ways[osm_id].back());

		for (osm_id_t next_id : graph[node_id])
			if (used.find(next_id) == used.end() && !ways[next_id].empty()) {
				if (nodes[ways[next_id].front()] == location) {
					save_locations(next_id, false);
				} else if (nodes[ways[next_id].back()] == location) {
					save_locations(next_id, true);
				}
			}
	}

	void relation_callback(osm_id_t osm_id, Tags const &tags, References const &refs)
	{
		bool boundary = is_boundary(tags);

		if (boundary) {
			locations.clear();

			graph.clear();
			used.clear();

			for (Reference const &r : refs) {
				if (is_way_ref(r) && !ways[r.member_id].empty()) {
					graph[ways[r.member_id].front()].push_back(r.member_id);
					graph[ways[r.member_id].back()].push_back(r.member_id);
				}
			}

			for (Reference const &r : refs) {
				if (is_way_ref(r) && used.find(r.member_id) == used.end())
					save_locations(r.member_id);
			}

			if (!locations.empty()) {
				std::cout << osm_id << ' ' << locations.size() << '\n';
				for (location_t const &l : locations)
					std::cout << l.lon << ' ' << l.lat << '\n';
			}
		}
	}
};

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(6);

	if (argc < 2) {
		log_error() << "osm-pbf-convert <geodata.osm.pbf>";
		return -1;
	}

	for (int i = 2; i < argc; ++i)
		debug_osm_ids.insert(atoll(argv[i]));

	need_ways_visit_t need_ways_visit;
	Parser<need_ways_visit_t>(argv[1], need_ways_visit).parse();

	log_info() << "Need ways parsed, count = " << need_ways.size();

	need_nodes_visit_t need_nodes_visit;
	Parser<need_nodes_visit_t>(argv[1], need_nodes_visit).parse();

	log_info() << "Need nodes parsed, count = " << need_nodes.size();

	parser_t parser;
	Parser<parser_t>(argv[1], parser).parse();

	return 0;
}
