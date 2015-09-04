#include "location.hpp"
#include "log.hpp"
#include "util.hpp"

#include <stdio.h>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <unordered_set>

using namespace geo_base;

static output_t &operator << (output_t &out, std::vector<location_t> const &locations)
{
	out << '[';
	for (size_t i = 0; i < locations.size(); ++i) {
		out << '(' << locations[i].lat << ' ' << locations[i].lon << ')';
		out << (i + 1 == locations.size() ? "" : ", ");
	}
	out << ']';
	return out;
}

int main(int argc, char *argv[])
{
	std::cout << std::fixed << std::setprecision(6);

	log_level(log_level_t::debug);
	log_debug("geo-base-grep") << "geo-base-grep <region_id> [Empty for all polygons]";

	std::unordered_set<region_id_t> grep;
	for (int i = 1; i < argc; ++i)
		grep.insert(atoll(argv[i]));

	try {
		std::vector<location_t> locations;

		proto_parser_t(STDIN_FILENO)([&] (proto::geo_data_t const &geo_data) {
			if (grep.empty() || grep.find(geo_data.region_id()) != grep.end()) {
				for (proto::polygon_t const &p : geo_data.polygons()) {
					locations.assign(p.locations().begin(), p.locations().end());
					polygon_processor_t()(locations, [&] (std::vector<location_t> const &l) {
						std::cout << l << std::endl;
					});
				}
			}
		});

	} catch (std::exception const &e) {
		log_error("geo-base-grep", "EXCEPTION") << e.what();
	}

	return 0;
}
