#include "location.h"
#include "log.h"
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

	if (argc != 2) {
		log_error() << "geo-base-grep <region_id>";
		return -1;
	}

	region_id_t grep_id = atoll(argv[1]);

	region_id_t region_id;
	count_t locations_count;

	while (std::cin >> region_id >> locations_count)
	{
		if (region_id == grep_id) {
			vector_t<location_t> raw_locations(locations_count);
			for (location_t &l : raw_locations)
				std::cin >> l.lon >> l.lat;

			vector_t<location_t> locations;
			for (location_t &l : raw_locations)
				if (locations.empty() || locations.back() != l)
					locations.push_back(l);

			std::cout << locations << std::endl;

		} else {
			location_t l;
			while (locations_count--)
				std::cin >> l.lon >> l.lat;
		}
	}

	return 0;
}
