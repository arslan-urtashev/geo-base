#include "geo_base.hpp"
#include "log.hpp"

#include <iomanip>
#include <iostream>

using namespace geo_base;

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(2);

	log_level(log_level_t::debug);

	if (argc != 2) {
		log_error("geo-base-show") << "geo-base-show <geodata.dat>";
		return -1;
	}

	try {
		geo_base_t geo_base(argv[1]);
		geo_base.show(std::cout);

	} catch (std::exception const &e) {
		log_error("geo-base-show", "EXCEPTION") << e.what();
	}

	return 0;
}
