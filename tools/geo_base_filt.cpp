#include "geo_base.h"
#include "unordered_set.h"
#include "log.h"

#include <iostream>
#include <random>
#include <thread>

using namespace troll;

static uint32_t const THREADS_COUNT = 20;
static uint32_t const LOOKUP_COUNT = 4000000000ULL;

struct worker_t {
	geo_base_t const *geo_base;
	std::mt19937 generator;
	unordered_set_t<region_id_t> regions;

	double get_rand()
	{
		return generator() * 1.0 / (generator.max() - generator.min());
	}

	double get_rand_lat()
	{
		return -90.0 + get_rand() * 180.0;
	}

	double get_rand_lon()
	{
		return -180.0 + get_rand() * 360.0;
	}

	worker_t(geo_base_t const *g = nullptr, uint32_t seed = 0)
		: geo_base(g)
		, generator(seed)
	{
	}

	void operator () ()
	{
		for (size_t i = 0; i < LOOKUP_COUNT; ++i) {
			location_t l(get_rand_lon(), get_rand_lat());
			region_id_t region_id = geo_base->lookup(l);
			regions.insert(region_id);
		}
	}
};

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	if (argc < 2) {
		log_error("geo-base-filt") << "geo-base-filt <geodata.dat>";
		return -1;
	}

	try {
		geo_base_t geo_base(argv[1]);

		std::mt19937 generator;

		vector_t<worker_t> workers(THREADS_COUNT);
		for (size_t i = 0; i < workers.size(); ++i)
			workers[i] = worker_t(&geo_base, generator());

		vector_t<std::thread> threads(THREADS_COUNT);
		for (size_t i = 0; i < threads.size(); ++i)
			threads[i] = std::thread(std::ref(workers[i]));

		for (size_t i = 0; i < threads.size(); ++i)
			threads[i].join();

		unordered_set_t<region_id_t> regions;
		for (size_t i = 0; i < workers.size(); ++i) {
			regions.insert(workers[i].regions.begin(), workers[i].regions.end());
			workers[i].regions.clear();
		}

		size_t count = 0;
		size_t filt_count = 0;

		geo_read_txt(std::cin, [&] (region_id_t region_id, vector_t<location_t> const &locations, vector_t<blob_t> const &blobs) {
			if (regions.find(region_id) != regions.end()) {
				std::cout << region_id << ' ' << locations.size() << ' ' << blobs.size() << '\n';
				for (location_t const &l : locations)
					std::cout << l.lat << ' ' << l.lon << '\n';
				for (blob_t const &b : blobs)
					std::cout << b << '\n';
			} else {
#if 0
				log_info("geo-base-filt") << "Filtered region = " << region_id;
				for (blob_t const &b : blobs)
					log_info("geo-base-filt") << "\t" << b;
#endif
				++filt_count;
			}
			++count;
		});

		log_info("geo-base-filt") << "Processed count = " << count;
		log_info("geo-base-filt") << "Filtered count = " << filt_count;

	} catch (std::exception const &e) {
		log_error("geo-base-filt", "EXCEPTION") << e.what();
	}
	
	return 0;
}
