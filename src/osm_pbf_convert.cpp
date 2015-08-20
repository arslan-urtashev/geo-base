#include "location.h"
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

static void usage()
{
	std::cerr << "osm-pbf-convert <geodata.osm>" << std::endl;
}

unordered_set_t<osm_id_t> need_ways;
unordered_set_t<osm_id_t> need_nodes;
unordered_map_t<osm_id_t, location_t> nodes;
unordered_map_t<osm_id_t, vector_t<osm_id_t>> ways;

struct need_ways_visit_t {
	void node_callback(osm_id_t, double , double , const Tags &)
	{
	}

	void way_callback(osm_id_t, Tags const &, std::vector<osm_id_t> const &)
	{
	}

	void relation_callback(osm_id_t, Tags const &tags, References const &refs)
	{
		bool boundary = false;
		for (auto const &p : tags)
			if (p.first == "admin_level" || (p.first == "boundary" && p.second == "administrative"))
				boundary = true;

		if (boundary) {
			for (Reference const &r : refs) 
				if (r.member_type == OSMPBF::Relation::WAY && r.role == "outer")
					need_ways.insert(r.member_id);
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

	void node_callback(osm_id_t osm_id, double lon, double lat, const Tags &)
	{
		if (need_nodes.find(osm_id) != need_nodes.end())
			nodes[osm_id] = location_t(lon, lat);
	}

	void way_callback(osm_id_t osm_id, Tags const &, std::vector<osm_id_t> const &nodes)
	{
		if (need_ways.find(osm_id) != need_ways.end())
			for (osm_id_t osm_id : nodes)
				need_nodes.insert(osm_id);
	}

	void relation_callback(osm_id_t osm_id, Tags const &tags, References const &refs)
	{
		bool boundary = false;
		for (auto const &p : tags)
			if (p.first == "admin_level" || (p.first == "boundary" && p.second == "administrative"))
				boundary = true;

		if (boundary) {
			locations.clear();
			for (Reference const &r : refs)
				if (r.member_type == OSMPBF::Relation::WAY && r.role == "outer")
					for (osm_id_t node_id : ways[r.member_id])
						locations.push_back(nodes[node_id]);
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

	if (argc != 2) {
		usage();
		return -1;
	}

	need_ways_visit_t need_ways_visit;
	Parser<need_ways_visit_t>(argv[1], need_ways_visit).parse();

	std::cerr << "Need ways parsed, count = " << need_ways.size() << std::endl;

	need_nodes_visit_t need_nodes_visit;
	Parser<need_nodes_visit_t>(argv[1], need_nodes_visit).parse();

	std::cerr << "Need nodes parsed, count = " << need_nodes.size() << std::endl;

	parser_t parser;
	Parser<parser_t>(argv[1], parser).parse();

	return 0;
}
