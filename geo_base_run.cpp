#include "geo_base.h"

#include <iostream>

static void usage()
{
	std::cerr << "geo-base-run <geodata.dat>" << std::endl;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	if (argc != 2) {
		usage();
		return -1;
	}

	try {
		troll::geo_base_t geo_base(argv[1]);

		double lon = 0, lat = 0;
		while (std::cin >> lon >> lat) {
			troll::location_t location(lon, lat);
			std::cout << geo_base.lookup(location) << '\n';
		}

	} catch (std::exception const &e) {
		std::cerr << "EXCEPTION: " << e.what() << std::endl;
	}

	return 0;
}
