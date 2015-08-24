#include <iomanip>
#include <iostream>
#include <sstream>

#include "generate.h"
#include "log.h"
#include "vector.h"

using namespace troll;

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	std::cout << std::fixed << std::setprecision(2);
	std::cerr << std::fixed << std::setprecision(6);

	if (argc != 2) {
		log_error("geo-base-generate") << "geo-base-generate <geodata.dat>";
		return -1;
	}

	try {
		generate_t generate(argv[1]);

		log_info("geo-base-generate") << "Run generate...";

		geo_read_txt(std::cin, [&] (region_id_t region_id, vector_t<location_t> const &locations, vector_t<blob_t> const &blobs) {
			generate.update(region_id, locations, blobs);
		});

		log_info("geo-base-generate") << "Save geodata...";

		generate.save();
		generate.show_base(std::cout);

		log_info("geo-base-generate") << "Done";

	} catch (std::exception const &e) {
		log_error("geo-base-generate", "EXCEPTION") << e.what();
	}

	return 0;
}
