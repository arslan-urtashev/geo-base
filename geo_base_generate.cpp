#include "generate.h"
#include "vector.h"

#include <iostream>

static void usage()
{
	std::cerr << "geo-base-generate <geo.dat>" << std::endl;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	if (argc != 2) {
		usage();
		return -1;
	}

	troll::generate_t generate(argv[1]);

	troll::vector_t<troll::location_t> locations;

	troll::region_id_t region_id;
	size_t locations_count = 0;

	while (std::cin >> region_id >> locations_count) {
		locations.resize(locations_count);
		for (troll::location_t &l : locations)
			std::cin >> l.lon >> l.lat;
		generate.update(region_id, locations);
	}

	generate.save();

	return 0;
}
