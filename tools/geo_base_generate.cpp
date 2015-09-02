#include <atomic>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "geo_base_generate.hpp"
#include "io.hpp"
#include "log.hpp"

using namespace geo_base;

typedef std::vector<location_t> locations_t;
typedef std::vector<std::string> blobs_t;

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
		proto_parser_t(STDIN_FILENO)([&] (proto::geo_data_t const &geo_data) {
			generate->update(geo_data);
		});

		readed = true;
	}
};

int main(int argc, char *argv[])
{
	std::cerr << std::fixed << std::setprecision(2);

	log_level(log_level_t::debug);

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
