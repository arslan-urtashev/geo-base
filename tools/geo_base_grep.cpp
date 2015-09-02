#include "location.hpp"
#include "log.hpp"
#include "util.hpp"

#include <stdio.h>
#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <unordered_set>

using namespace geo_base;

static output_t &operator << (output_t &out, proto::polygon_t const &p)
{
	out << '[';
	for (int i = 0; i < p.locations_size(); ++i) {
		out << '(' << p.locations(i).lat() << ' ' << p.locations(i).lon() << ')';
		out << (i + 1 == p.locations_size() ? "" : ", ");
	}
	out << ']';
	return out;
}

int main(int argc, char *argv[])
{
	std::cout << std::fixed << std::setprecision(6);

	log_level(log_level_t::debug);

	if (argc < 2) {
		log_error("geo-base-grep") << "geo-base-grep <region_id>";
		return -1;
	}

	std::unordered_set<region_id_t> grep;
	for (int i = 1; i < argc; ++i)
		grep.insert(atoll(argv[i]));

	try {
		proto_parser_t(STDIN_FILENO)([&] (proto::geo_data_t const &geo_data) {
			if (grep.find(geo_data.region_id()) != grep.end())
				for (proto::polygon_t const &p : geo_data.polygons())
					std::cout << p << std::endl;
		});

	} catch (std::exception const &e) {
		log_error("geo-base-grep", "EXCEPTION") << e.what();
	}

	return 0;
}
