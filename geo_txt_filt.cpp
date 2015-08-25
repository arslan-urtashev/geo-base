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

static double const LOOKUP_RADIUS = 0.001;

static uint32_t const THREADS_COUNT = 2;
static uint32_t const LOOKUP_COUNT = 1000;

struct worker_t {
	geo_base_t const *geo_base;
	std::mt19937 generator;
	unordered_set_t<region_id_t> regions;
	count_t points_offset;
	count_t points_count;
	std::atomic<count_t> offset;

	double get_rand()
	{
		return -1.0 + generator() * 2.0 / (generator.max() - generator.min());
	}

	worker_t(geo_base_t const *g = nullptr, offset_t points_offset = 0, count_t points_count = 0, uint32_t seed = 0)
		: geo_base(g)
		, generator(seed)
		, points_offset(points_offset)
		, points_count(points_count)
		, offset(points_offset)
	{
	}

	worker_t& operator = (worker_t const &w)
	{
		geo_base = w.geo_base;
		regions = w.regions;
		generator = w.generator;
		points_offset = w.points_offset;
		points_count = w.points_count;
		offset = points_offset;
		return *this;
	}

	void operator () ()
	{
		for (offset = points_offset; offset < points_offset + points_count; ++offset) {
			point_t const &p = geo_base->geo_data()->points[offset];

			double lon = convert_to_double(p.x) + get_rand() * LOOKUP_RADIUS;
			double lat = convert_to_double(p.y) + get_rand() * LOOKUP_RADIUS;

			region_id_t region_id = geo_base->lookup(location_t(lon, lat));

			regions.insert(region_id);
		}
	}
};

static output_t &operator << (output_t &out, worker_t const &w)
{
	out << (w.offset - w.points_offset) * 100.0 / w.points_count << "%";
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

		count_t points_count = geo_base.geo_data()->points_count;
		count_t one_thread_count = points_count / THREADS_COUNT + 1;

		vector_t<worker_t> workers(THREADS_COUNT);
		for (size_t i = 0, offset = 0; i < THREADS_COUNT && offset < points_count; ++i, offset += one_thread_count)
			workers[i] = worker_t(&geo_base, offset, min(one_thread_count, points_count - offset), i);

		vector_t<std::thread> threads(THREADS_COUNT);
		for (size_t i = 0; i < threads.size(); ++i)
			threads[i] = std::thread(std::ref(workers[i]));

		while (true) {
			bool completed = true;

			for (size_t i = 0; i < workers.size(); ++i)
				if (workers[i].offset != workers[i].points_offset + workers[i].points_count)
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
