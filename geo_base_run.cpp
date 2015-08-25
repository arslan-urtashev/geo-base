#include "geo_base.h"
#include "log.h"

#include <iomanip>
#include <iostream>

using namespace troll;

static input_t &operator >> (input_t &in, location_t &l)
{
	in >> l.lat;
	in.ignore();
	in >> l.lon;
	return in;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	if (argc != 2) {
		log_error("geo-base-run")  << "geo-base-run <geodata.dat>";
		return -1;
	}

	try {
		geo_base_t geo_base(argv[1]);

		location_t location;
		while (std::cin >> location) {
			region_id_t region_id = geo_base.lookup(location);

			if (region_id != UNKNOWN_REGION_ID) {
				std::cout << region_id << '\n';
				geo_base.kv_each(region_id, [&] (char const *k, char const *v) {
					std::cout << "  " << k << " = " << v << '\n';
				});
			} else {
				std::cout << -1 << '\n';
			}
		}

	} catch (std::exception const &e) {
		log_error("geo-base-run", "EXCEPTION") << e.what();
	}

	return 0;
}
