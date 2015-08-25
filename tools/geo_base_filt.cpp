#include "geo_base.h"
#include "unordered_set.h"
#include "log.h"

#include <atomic>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

using namespace troll;

static uint32_t const THREADS_COUNT = 2;
static uint32_t const LOOKUP_COUNT = 4000000000ULL;

struct worker_t {
	geo_base_t const *geo_base;
	unordered_set_t<region_id_t> regions;
	std::mt19937 generator;
	std::atomic<uint32_t> counter;

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
		, counter(0)
	{
	}

	worker_t& operator = (worker_t const &w)
	{
		geo_base = w.geo_base;
		regions = w.regions;
		generator = w.generator;
		counter = 0;
		return *this;
	}

	void operator () ()
	{
		for (counter = 0; counter < LOOKUP_COUNT; ++counter) {
			location_t l(get_rand_lon(), get_rand_lat());
			region_id_t region_id = geo_base->lookup(l);
			regions.insert(region_id);
		}
	}
};

static output_t &operator << (output_t &out, worker_t const &w)
{
	out << w.counter * 100.0 / LOOKUP_COUNT << "%";
	return out;
}

static output_t &operator << (output_t &out, vector_t<worker_t> const &w)
{
	for (worker_t const &worker : w)
		out << worker << " ";
	return out;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cerr << std::fixed << std::setprecision(2);

	if (argc < 2) {
		log_error("geo-base-filt") << "geo-base-filt <geodata.dat>";
		return -1;
	}

	try {
		geo_base_t geo_base(argv[1]);

		vector_t<worker_t> workers(THREADS_COUNT);
		for (size_t i = 0; i < THREADS_COUNT; ++i)
			workers[i] = worker_t(&geo_base, i);

		vector_t<std::thread> threads(THREADS_COUNT);
		for (size_t i = 0; i < threads.size(); ++i)
			threads[i] = std::thread(std::ref(workers[i]));

		while (true) {
			bool completed = true;

			for (size_t i = 0; i < workers.size(); ++i)
				if (workers[i].counter != LOOKUP_COUNT)
					completed = false;

			log_info("geo-base-filt", "status") << workers;

			if (completed)
				break;

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

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
