#include <iostream>

#include "algo.h"
#include "generate.h"
#include "geo_base_alloc.h"
#include "library.h"
#include "vector.h"

static void usage()
{
	std::cerr << "geo-base-generate <geodata.dat>" << std::endl;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	if (argc != 2) {
		usage();
		return -1;
	}

	try {
		troll::generate_t generate(argv[1]);

		troll::region_id_t region_id;
		troll::count_t locations_count;
		troll::vector_t<troll::location_t> locations;

		while (std::cin >> region_id >> locations_count) {
			locations.resize(locations_count);
			for (troll::location_t &l : locations) {
				if (!(std::cin >> l.lon >> l.lat)) {
					std::cerr << "Wrong locations count" << std::endl;
					return -1;
				}
			}
			generate.update(region_id, locations);
		}

		generate.save();
		generate.show_base(std::cout);

	} catch (std::exception const &e) {
		std::cerr << "EXCEPTION: " << e.what() << std::endl;
	}

	return 0;
}
