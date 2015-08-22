#include <iomanip>
#include <iostream>
#include <sstream>

#include "algo.h"
#include "generate.h"
#include "geo_base_alloc.h"
#include "library.h"
#include "log.h"
#include "vector.h"
#include "watch.h"

using namespace troll;

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	std::cout << std::fixed << std::setprecision(2);
	std::cerr << std::fixed << std::setprecision(6);

	if (argc != 2) {
		log_error() << "geo-base-generate <geodata.dat>";
		return -1;
	}

	try {
		generate_t generate(argv[1]);

		region_id_t region_id;
		count_t locations_count = 0;
		vector_t<location_t> locations;

		std::string curstr;
		std::stringstream strin;

		count_t lines_count = 0;
		watch_t watch;

		while (std::getline(std::cin, curstr)) {
			++lines_count;
			count_t region_line = lines_count;

			strin.clear();
			strin << curstr;

			if (!(strin >> region_id)) {
				log_error("geo-base-generate") << "Can't read region_id on "
					<< lines_count << ": \"" << curstr << "\"";
				return -1;
			}

			if (!(strin >> locations_count)) {
				log_error("geo-base-generate") << "Can't read locations count on "
					<< lines_count << ": \"" << curstr << "\"";
				return -1;
			}

			locations.resize(locations_count);

			for (location_t &l : locations) {
				++lines_count;
				
				if (!std::getline(std::cin, curstr)) {
					log_error("geo-base-generate") << "Wron locations count for "
						<< region_id << " on " << region_line;
					return -1;
				}

				strin.clear();
				strin << curstr;

				if (!(strin >> l.lon >> l.lat)) {
					log_error("geo-base-generate") << "Can't read locations on "
						<< lines_count << ": \"" << curstr << "\"";
					return -1;
				}
			}

			generate.update(region_id, locations);
		}

		generate.save();
		generate.show_base(std::cout);

		log_info("geo-base-generate") << "Processed lines count = " << lines_count;
		log_info("geo-base-generate") << "Generated for " << watch.total() / 60. << " minutes";

	} catch (std::exception const &e) {
		log_error() << "EXCEPTION: " << e.what();
	}

	return 0;
}
