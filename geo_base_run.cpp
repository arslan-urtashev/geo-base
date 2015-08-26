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

static char const *get(char const *n1, char const *n2, char const *n3)
{
	if (n1)
		return n1;
	if (n2)
		return n2;
	if (n3)
		return n3;
	return "-- unknown --";
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	log_level(log_level_t::error);

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
				char const *n1 = nullptr;
				char const *n2 = nullptr;
				char const *n3 = nullptr;
				geo_base.kv_each(region_id, [&] (char const *k, char const *v) {
					log_debug("geo-base-run", "kv_each") << '"' << k << '"' << ' ' << '"' << v << '"';
					if (strstr(k, "name")) {
						n3 = v;
						if (!strcmp(k, "name"))
							n1 = v;
						else if (!strcmp(k, "name:en"))
							n2 = v;
					}
				});
				std::cout << region_id << " ";
				std::cout << '"' << get(n1, n2, n3) << '"';
				if (n2)
					std::cout << " (\"" << n2 << "\")";
				std::cout << '\n';
			} else {
				std::cout << -1 << '\n';
			}
		}

	} catch (std::exception const &e) {
		log_error("geo-base-run", "EXCEPTION") << e.what();
	}

	return 0;
}
