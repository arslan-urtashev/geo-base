#include <iomanip>
#include <iostream>

#include "algo.h"
#include "generate.h"
#include "geo_base_alloc.h"
#include "library.h"
#include "log.h"
#include "vector.h"

using namespace troll;

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(2);

	if (argc != 2) {
		log_error() << "geo-base-generate <geodata.dat>";
		return -1;
	}

	try {
		generate_t generate(argv[1]);

		region_id_t region_id;
		count_t locations_count;
		vector_t<location_t> locations;

		while (std::cin >> region_id >> locations_count) {
			locations.resize(locations_count);
			for (location_t &l : locations) {
				if (!(std::cin >> l.lon >> l.lat)) {
					log_error() << "Wrong locations count";
					return -1;
				}
			}
			generate.update(region_id, locations);
		}

		generate.save();
		generate.show_base(std::cout);

	} catch (std::exception const &e) {
		log_error() << "EXCEPTION: " << e.what();
	}

	return 0;
}
