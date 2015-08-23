#include "geo_read_txt.h"
#include "location.h"
#include "log.h"
#include "unordered_set.h"
#include "vector.h"

#include <iomanip>
#include <iostream>

using namespace troll;

static output_t &operator << (output_t &out, vector_t<location_t> const &l)
{
	out << '[';
	for (size_t i = 0; i < l.size(); ++i)
		out << '(' << l[i].lat << ' ' << l[i].lon << ')' << (i + 1 == l.size() ? "" : ", ");
	out << ']';
	return out;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(6);

	if (argc < 2) {
		log_error("geo-base-grep") << "geo-base-grep <region_id>";
		return -1;
	}

	unordered_set_t<region_id_t> grep;
	for (int i = 1; i < argc; ++i)
		grep.insert(atoll(argv[i]));

	try {
		vector_t<location_t> locations;
		vector_t<location_t> polygon;

		geo_read_txt(
			std::cin,
			[&] (region_id_t region_id, vector_t<location_t> const &raw_locations)
			{
				if (grep.find(region_id) != grep.end()) {
					locations.clear();
					for (location_t const &l : raw_locations)
						if (locations.empty() || locations.back() != l)
							locations.push_back(l);

					for (ref_t l = 0, r = 0; l < locations.size(); l = r + 1) {
						r = l + 1;
						while (r < locations.size() && locations[l] != locations[r])
							++r;

						polygon.clear();
						for (ref_t i = l; i < r; ++i)
							polygon.push_back(locations[i]);

						if (grep.size() != 1)
							std::cout << region_id << " = ";
						std::cout << polygon << std::endl;
					}
				}
			}
		);

	} catch (std::exception const &e) {
		log_error("geo-base-grep", "EXCEPTION") << e.what();
	}

	return 0;
}
