#include "geo_base.h"
#include "log.h"
#include "watch.h"

#include <iomanip>
#include <iostream>

using namespace troll;

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cerr << std::fixed << std::setprecision(3);

	if (argc != 2) {
		log_error()  << "geo-base-run <geodata.dat>";
		return -1;
	}

	try {
		geo_base_t geo_base(argv[1]);
		geo_base.show(std::cerr);

		double longest = 0;
		count_t counter = 0;

		watch_t watch;
		location_t location;

		while (std::cin >> location.lon >> location.lat) {
			watch.checkpoint();
			std::cout << geo_base.lookup(location) << '\n';
			longest = max(longest, watch.checkpoint());
			++counter;
		}

		double total = watch.total();

		log_info() << "COUNT = " << counter;
		log_info() << "SPENT TIME:";
		log_info() << "  TOTAL = " << total << " S";
		log_info() << "  AVG = " << total * 1000. / counter << " MS";
		log_info() << "  LONGEST = " << longest * 1000. << " MS";

	} catch (std::exception const &e) {
		log_error() << "EXCEPTION: " << e.what();
	}

	return 0;
}
