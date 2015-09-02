#include "geo_base.hpp"
#include "log.hpp"

#include <iomanip>
#include <iostream>

using namespace geo_base;

struct region_out_t {
	region_id_t region_id;
	geo_base_t const &geo_base;
	char const *name1;
	char const *name2;

	region_out_t(region_id_t region_id, geo_base_t const &geo_base)
		: region_id(region_id)
		, geo_base(geo_base)
	{
	}
};

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

static void get_name(region_id_t region_id, geo_base_t const &geo_base, char const **name1, char const **name2)
{
	char const *n1 = nullptr;
	char const *n2 = nullptr;
	char const *n3 = nullptr;
	
	geo_base.kv_each(region_id, [&] (char const *k, char const *v) {
		if (strstr(k, "name")) {
			n3 = v;
			if (!strcmp(k, "name"))
				n1 = v;
			else if (!strcmp(k, "name:en"))
				n2 = v;
		}
	});

	*name1 = get(n1, n2, n3);
	*name2 = n2 ? n2 : nullptr; 
}

static output_t &operator << (output_t &out, region_out_t const &r)
{
	char const *name1 = nullptr;
	char const *name2 = nullptr;

	get_name(r.region_id, r.geo_base, &name1, &name2);

	out << r.region_id << " ";
	out << '"' << name1 << '"';
	if (name2)
		out << " (\"" << name2 << "\")";

	return out;
}

int main(int argc, char *argv[])
{
	log_level(log_level_t::debug);

	if (argc != 2) {
		log_error("geo-base-run")  << "geo-base-run <geodata.dat>";
		return -1;
	}

	try {
		geo_base_t geo_base(argv[1]);

		location_t location;
		std::vector<region_id_t> regs;

		while (std::cin >> location) {
			region_id_t region_id = geo_base.lookup(location, &regs);

			if (region_id != UNKNOWN_REGION_ID) {
				std::cout << region_out_t(region_id, geo_base) << '\n';
				for (region_id_t const &r : regs)
					log_debug("geo-base-run", region_id) << region_out_t(r, geo_base);
			} else {
				std::cout << -1 << '\n';
			}
		}

	} catch (std::exception const &e) {
		log_error("geo-base-run", "EXCEPTION") << e.what();
	}

	return 0;
}
