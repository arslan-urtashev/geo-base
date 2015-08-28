#include "common.hpp"
#include "location.hpp"
#include "log.hpp"

#include "osmpbfreader/osmpbfreader.h"

#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace geo_base;
using namespace CanalTP;

typedef uint64_t osm_id_t;

std::unordered_set<osm_id_t> debug_osm_ids;
std::unordered_set<osm_id_t> need_ways;
std::unordered_set<osm_id_t> need_nodes;
std::unordered_map<osm_id_t, location_t> nodes;
std::unordered_map<osm_id_t, std::vector<osm_id_t>> ways;

static bool is_boundary(Tags const &tags)
{
	bool ok = false;
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
		) {
			ok = true;
			break;
		}
	if (!ok)
		return false;
	ok = false;
	for (auto const &p : tags)
		if (p.first.find("name") == 0LLU) {
			ok = true;
			break;
		}
	return ok;
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

	void way_callback(osm_id_t osm_id, Tags const &tags, std::vector<osm_id_t> const &refs)
	{
		if (is_boundary(tags) && refs.front() == refs.back())
			need_ways.insert(osm_id);
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
	std::vector<location_t> locations;

	std::unordered_map<osm_id_t, std::vector<osm_id_t>> graph;
	std::unordered_set<osm_id_t> used;

	void node_callback(osm_id_t osm_id, double lon, double lat, const Tags &)
	{
		if (need_nodes.find(osm_id) != need_nodes.end())
			nodes[osm_id] = location_t(lon, lat);
	}

	void way_callback(osm_id_t osm_id, Tags const &tags, std::vector<osm_id_t> const &refs)
	{
		if (is_boundary(tags) && refs.back() == refs.front()) {
			std::cout << osm_id << ' ' << refs.size() << ' ' << 2 * tags.size() << '\n';
			for (osm_id_t osm_id : refs)
				std::cout << nodes[osm_id].lat << ' ' << nodes[osm_id].lon << '\n';
			for (auto const &p : tags)
				std::cout << p.first << '\n' << p.second << std::endl;
		}
	}

	void save_locations(osm_id_t osm_id)
	{
		used.insert(osm_id);
		locations.push_back(nodes[osm_id]);

		for (osm_id_t node_id : graph[osm_id])
			if (used.find(node_id) == used.end())
				save_locations(node_id);
	}

	void relation_callback(osm_id_t osm_id, Tags const &tags, References const &refs)
	{
		bool boundary = is_boundary(tags);

		if (boundary) {
			locations.clear();

			graph.clear();
			used.clear();

			for (Reference const &r : refs) {
				if (is_way_ref(r)) {
					std::vector<osm_id_t> const &w = ways[r.member_id];
					for (size_t i = 0; i + 1 < w.size(); ++i) {
						graph[w[i]].push_back(w[i + 1]);
						graph[w[i + 1]].push_back(w[i]);
					}
				}
			}

			for (Reference const &r : refs) {
				if (is_way_ref(r)) {
					std::vector<osm_id_t> const &w = ways[r.member_id];
					for (size_t i = 0; i < w.size(); ++i) {
						if (used.find(w[i]) == used.end()) {
							save_locations(w[i]);
							locations.push_back(nodes[w[i]]);
						}
					}
				}
			}

			if (!locations.empty()) {
				std::cout << osm_id << ' ' << locations.size() << ' ' << 2 * tags.size() << std::endl;
				for (location_t const &l : locations)
					std::cout << l.lat << ' ' << l.lon << std::endl;
				for (auto const &p : tags)
					std::cout << p.first << std::endl << p.second << std::endl;
				std::cout.flush();
			}
		}
	}
};

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(6);

	if (argc < 2) {
		log_error("geo-base-convert") << "geo-base-convert <geodata.osm.pbf>";
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
