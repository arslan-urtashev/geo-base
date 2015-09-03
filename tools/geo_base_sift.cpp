#include "geo_base.hpp"
#include "log.hpp"

#include <atomic>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include <unordered_map>
#include <unordered_set>

using namespace geo_base;

static double const LOOKUP_RADIUS = 0.001;

static uint32_t const THREADS_COUNT = 2;
static uint32_t const LOOKUP_COUNT = 1000;

struct worker_t {
	geo_base_t const *geo_base;
	std::mt19937 generator;
	std::unordered_set<polygon_id_t> polygons;
	count_t points_offset;
	count_t points_count;
	std::atomic<count_t> offset;

	double get_rand()
	{
		return -1.0 + generator() * 2.0 / (generator.max() - generator.min());
	}

	worker_t(geo_base_t const *g = nullptr, count_t points_offset = 0, count_t points_count = 0, uint32_t seed = 0)
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
		polygons = w.polygons;
		generator = w.generator;
		points_offset = w.points_offset;
		points_count = w.points_count;
		offset = points_offset;
		return *this;
	}

	void operator () ()
	{
		geo_base_t::debug_t debug;
		for (offset = points_offset; offset < points_offset + points_count; ++offset) {
			for (size_t try_lookup = 0; try_lookup < LOOKUP_COUNT; ++try_lookup) {
				point_t const &p = geo_base->geo_data()->points[offset];

				double lon = convert_to_double(p.x) + get_rand() * LOOKUP_RADIUS;
				double lat = convert_to_double(p.y) + get_rand() * LOOKUP_RADIUS;

				region_id_t region_id = geo_base->lookup(location_t(lon, lat), &debug);

				if (region_id != UNKNOWN_REGION_ID)
					polygons.insert(debug.polygon_id);
			}
		}
	}
};

#ifdef GEO_TXT_FILT_PERCENT

static output_t &operator << (output_t &out, worker_t const &w)
{
	out << (w.offset - w.points_offset) * 100.0 / w.points_count << "%";
	return out;
}

static output_t &operator << (output_t &out, std::vector<worker_t> const &workers)
{
	for (worker_t const &w : workers)
		out << w << " ";
	return out;
}

#else

static output_t &operator << (output_t &out, std::vector<worker_t> const &workers)
{
	count_t count = 0, total = 0;
	for (worker_t const &w : workers) {
		count += (w.offset - w.points_offset);
		total += w.points_count;
	}
	count_t result = count * 100.0 / total;
	out << "[";
	for (count_t i = 1; i <= 100; ++i)
		out << (i <= result ? "#" : " ");
	out << "]";
	return out;
}

#endif

int main(int argc, char *argv[])
{
	std::cerr << std::fixed << std::setprecision(2);

	log_level(log_level_t::debug);

	if (argc < 2) {
		log_error("geo-base-sift") << "geo-base-sift <geodata.dat>";
		return -1;
	}

	try {
		geo_base_t geo_base(argv[1]);

		size_t points_count = geo_base.geo_data()->points_count;
		size_t one_thread_count = points_count / THREADS_COUNT + 1;

		std::vector<worker_t> workers(THREADS_COUNT);
		for (size_t i = 0, offset = 0; i < THREADS_COUNT && offset < points_count; ++i, offset += one_thread_count)
			workers[i] = worker_t(&geo_base, offset, std::min(one_thread_count, points_count - offset), i);

		std::vector<std::thread> threads(THREADS_COUNT);
		for (size_t i = 0; i < threads.size(); ++i)
			threads[i] = std::thread(std::ref(workers[i]));

		while (true) {
			bool completed = true;

			for (size_t i = 0; i < workers.size(); ++i)
				if (workers[i].offset != workers[i].points_offset + workers[i].points_count)
					completed = false;

			status("geo-base-sift", "monte-carlo") << workers;

			if (completed)
				break;

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		for (size_t i = 0; i < threads.size(); ++i)
			threads[i].join();
		status::clear();

		std::unordered_set<polygon_id_t> polygons;
		for (size_t i = 0; i < workers.size(); ++i) {
			polygons.insert(workers[i].polygons.begin(), workers[i].polygons.end());
			workers[i].polygons.clear();
		}

		size_t count = 0;
		size_t filt_count = 0;

		proto_writer_t writer(STDOUT_FILENO);

		proto::geo_data_t buf_geo_data;
		std::string buf;

		proto_parser_t(STDIN_FILENO)([&] (proto::geo_data_t const &geo_data) {
			buf_geo_data = geo_data;

			buf_geo_data.mutable_polygons()->Clear();
			for (proto::polygon_t const &p : geo_data.polygons()) {
				if (polygons.find(p.polygon_id()) != polygons.end()) {
					*buf_geo_data.add_polygons() = p;
				} else {
					++filt_count;
				}
				++count;
			}

			writer.write(buf_geo_data, buf);
		});

		log_info("geo-base-sift") << "All polygons count = " << count;
		log_info("geo-base-sift") << "Sift polygons count = " << filt_count;

	} catch (std::exception const &e) {
		log_error("geo-base-sift", "EXCEPTION") << e.what();
	}
	
	return 0;
}
