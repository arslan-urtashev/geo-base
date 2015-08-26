#include <atomic>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "geo_base_generate.h"
#include "io.h"
#include "log.h"
#include "vector.h"

using namespace troll;

using locations_t = vector_t<location_t>;
using blobs_t = vector_t<blob_t>;

struct reader_t {
	geo_base_generate_t *generate;
	std::atomic<bool> readed;

	reader_t(geo_base_generate_t *g)
		: generate(g)
		, readed(false)
	{
	}

	void operator () ()
	{
		geo_read_txt(std::cin, [&] (region_id_t r, locations_t const &l, blobs_t const &b) {
			generate->update(r, l, b);
		});

		readed = true;
	}
};

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	std::cout << std::fixed << std::setprecision(2);
	std::cerr << std::fixed << std::setprecision(2);

	if (argc != 2) {
		log_error("geo-base-generate") << "geo-base-generate <geodata.dat>";
		return -1;
	}

	try {
		stop_watch_t stop_watch;
		stop_watch.checkpoint();

		geo_base_generate_t generate(argv[1]);

		log_info("geo-base-generate") << "Run generate...";

		reader_t reader(&generate);
		std::thread thread(std::ref(reader));

		while (!reader.readed) {
			geo_data_ctx_t const &ctx = generate.context();
			status("geo-base-generate", "status") << "Polygons = " << ctx.polygons.size() << ", " << ctx.memory() << " MB";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		thread.join();
		status::clear();

		log_info("geo-base-generate") << "Save geodata...";

		generate.save();
		generate.show_base(std::cout);

		log_info("geo-base-generate") << "Done, spent = " << stop_watch.checkpoint() * 1.0 / 60. << " MIN";

	} catch (std::exception const &e) {
		log_error("geo-base-generate", "EXCEPTION") << e.what();
	}

	return 0;
}
