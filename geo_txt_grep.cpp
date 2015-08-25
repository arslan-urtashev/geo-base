#include "location.h"
#include "log.h"
#include "unordered_set.h"
#include "vector.h"
#include "util.h"

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
		log_error("geo-txt-grep") << "geo-txt-grep <region_id>";
		return -1;
	}

	unordered_set_t<region_id_t> grep;
	for (int i = 1; i < argc; ++i)
		grep.insert(atoll(argv[i]));

	try {
		vector_t<location_t> locations;
		vector_t<location_t> polygon;

		geo_read_txt(std::cin, [&] (region_id_t region_id, vector_t<location_t> const &raw_locations, vector_t<blob_t> const &) {
			if (grep.find(region_id) != grep.end()) {
				process_locations(raw_locations, locations, [&] (vector_t<location_t> const &locations) {
					if (grep.size() != 1)
						std::cout << region_id << " = ";
					std::cout << locations << std::endl;
				});
			}
		});

	} catch (std::exception const &e) {
		log_error("geo-txt-grep", "EXCEPTION") << e.what();
	}

	return 0;
}
