#include "location.h"
#include "typedef.h"
#include "unordered_map.h"
#include "unordered_set.h"
#include "vector.h"

#include <string.h>

#include <fstream>
#include <iomanip>
#include <iostream>

using namespace troll;

using osm_id_t = int64_t;

template<typename callback_t>
void run(char const *path, callback_t &callback)
{
	std::ifstream in(path);

	std::string cur;
	while (std::getline(in, cur))
		callback(cur.c_str());
}

static char const *go(char const *str, char const *to)
{
	char const *ptr = strstr(str, to);
	return ptr ? ptr + strlen(to) : '\0';
}

struct ways_saver_t {
	unordered_set_t<osm_id_t> ways;	
	vector_t<osm_id_t> current_ways;

	bool boundary;

	ways_saver_t()
		: boundary(false)
	{
	}

	void operator () (char const *str)
	{
		if (strstr(str, "<relation")) {
			boundary = false;
			current_ways.clear();

		} else if (strstr(str, "<tag k=\"boundary\" v=\"administrative\"/>") || strstr(str, "<tag k=\"admin_level\"")) {
			boundary = true;

		} else if (strstr(str, "<member type=\"way\" ref=\"") && strstr(str, "role=\"outer\"/>")) {
			current_ways.push_back(atoll(go(str, "<member type=\"way\" ref=\"")));

		} if (strstr(str, "</relation")) {
			if (boundary)
				for (osm_id_t osm_id : current_ways)
					ways.insert(osm_id);
		}
	}
};

struct nodes_saver_t {
	unordered_set_t<osm_id_t> const &ways;
	unordered_set_t<osm_id_t> nodes;
	vector_t<osm_id_t> current_nodes;

	osm_id_t current_osm_id;

	nodes_saver_t(ways_saver_t const &ways_saver)
		: ways(ways_saver.ways)
	{
	}

	void operator () (char const *str)
	{
		if (strstr(str, "<way id=\"")) {
			current_nodes.clear();
			current_osm_id = atoll(go(str, "<way id=\""));

		} else if (strstr(str, "<nd ref=\"")) {
			current_nodes.push_back(atoll(go(str, "<nd ref=\"")));

		} else if (strstr(str, "</way>")) {
			if (ways.find(current_osm_id) != ways.end())
				for (osm_id_t osm_id : current_nodes)
					nodes.insert(osm_id);
		}
	}
};

struct parser_t {
	unordered_set_t<osm_id_t> const &saved_ways;
	unordered_set_t<osm_id_t> const &saved_nodes;

	unordered_map_t<osm_id_t, location_t> nodes;
	unordered_map_t<osm_id_t, vector_t<osm_id_t>> ways;

	vector_t<osm_id_t> relation;
	vector_t<location_t> locations;

	osm_id_t current_osm_id;
	bool boundary;

	parser_t(ways_saver_t const &ways_saver, nodes_saver_t const &nodes_saver)
		: saved_ways(ways_saver.ways)
		, saved_nodes(nodes_saver.nodes)
		, current_osm_id(-1)
		, boundary(false)
	{
	}

	void operator () (char const *str)
	{
		if (strstr(str, "<node id=\"")) {
			osm_id_t node_osm_id = atoll(go(str, "<node id=\""));
			if (saved_nodes.find(node_osm_id) != saved_nodes.end()) {
				location_t l;
				l.lon = strtod(go(str, " lon=\""), nullptr);
				l.lat = strtod(go(str, " lat=\""), nullptr);
				nodes[node_osm_id] = l;
			}

		} else if (strstr(str, "<way id=\"")) {
			current_osm_id = atoll(go(str, "<way id=\""));
			if (saved_ways.find(current_osm_id) == saved_ways.end())
				current_osm_id = -1;

		} else if (strstr(str, "<nd ref=\"")) {
			if (current_osm_id != -1)
				ways[current_osm_id].push_back(atoll(go(str, "<nd ref=\"")));

		} else if (strstr(str, "</way>")) {
			current_osm_id = -1;

		} else if (strstr(str, "<relation id=\"")) {
			current_osm_id = atoll(go(str, "<relation id=\""));
			boundary = false;

		} else if (strstr(str, "</relation>")) {
			if (boundary) {
				locations.clear();
				for (osm_id_t way_id : relation)
					for (osm_id_t node_id : ways[way_id])
						locations.push_back(nodes[node_id]);

				std::cout << current_osm_id << ' ' << locations.size() << '\n';
				for (location_t const &l : locations)
					std::cout << l.lon << ' ' << l.lat << '\n';
			}

			current_osm_id = -1;
			relation.clear();

		} else if (strstr(str, "<tag k=\"boundary\" v=\"administrative\"/>") || strstr(str, "<tag k=\"admin_level\"")) {
			boundary = true;

		} else if (strstr(str, "<member type=\"way\" ref=\"") && strstr(str, "role=\"outer\"/>")) {
			relation.push_back(atoll(go(str, "<member type=\"way\" ref=\"")));
		}
	}
};

static void usage()
{
	std::cerr << "geo-osm-convert <data.osm>" << std::endl;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(6);

	if (argc != 2) {
		usage();
		return -1;
	}

	ways_saver_t ways_saver;
	run(argv[1], ways_saver);

	std::cerr << "Ways save, total = " << ways_saver.ways.size() << std::endl;

	nodes_saver_t nodes_saver(ways_saver);
	run(argv[1], nodes_saver);

	std::cerr << "Nodes save, total = " << nodes_saver.nodes.size() << std::endl;

	parser_t parser(ways_saver, nodes_saver);
	run(argv[1], parser);

	return 0;
}
