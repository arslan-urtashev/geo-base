#include "geo_base.h"
#include "watch.h"

#include <iomanip>
#include <iostream>

static void usage()
{
	std::cerr << "geo-base-run <geodata.dat>" << std::endl;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cerr << std::fixed << std::setprecision(3);

	if (argc != 2) {
		usage();
		return -1;
	}

	try {
		troll::geo_base_t geo_base(argv[1]);
		geo_base.show(std::cerr);

		double longest = 0;
		troll::count_t counter = 0;

		troll::watch_t watch;
		troll::location_t location;

		while (std::cin >> location.lon >> location.lat) {
			watch.checkpoint();
			std::cout << geo_base.lookup(location) << '\n';
			longest = troll::max(longest, watch.checkpoint());
			++counter;
		}

		double total = watch.total();

		std::cerr << "SPENT TIME:" << std::endl;
		std::cerr << "  TOTAL = " << total << " S" << std::endl;
		std::cerr << "  AVG = " << total * 1000. / counter << " MS" << std::endl;
		std::cerr << "  LONGEST = " << longest * 1000. << " MS" << std::endl;

	} catch (std::exception const &e) {
		std::cerr << "EXCEPTION: " << e.what() << std::endl;
	}

	return 0;
}
