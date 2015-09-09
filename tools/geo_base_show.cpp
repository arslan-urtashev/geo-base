#include "geo_base.hpp"
#include "log.hpp"
#include "hash.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace geo_base;

template<typename callback_t>
static void for_each_polygon(geo_data_t const *dat, callback_t callback)
{
	for (count_t i = 0; i < dat->polygons_count; ++i)
		callback(&(dat->polygons[i]));
}

template<typename callback_t>
static void for_each_part(geo_data_t const *dat, polygon_t const *p, callback_t callback)
{
	for (count_t i = p->parts_offset; i < p->parts_offset + p->parts_count; ++i)
		callback(&(dat->parts[i]));
}

template<typename callback_t>
static void for_each_ref(geo_data_t const *dat, part_t const *part, callback_t callback)
{
	for (count_t i = part->edge_refs_offset; i < (part + 1)->edge_refs_offset; ++i)
		callback(dat->edge_refs[i]);
}

template<typename callback>
static void for_each(geo_data_t const *dat)
{
	for_each_polygon(dat, [&] (polygon_t const *polygon) {
		for_each_part(dat, polygon, [&] (part_t const *part) {
			for_each_ref(dat, part, [&] (ref_t const &ref) {
				callback(polygon, part, ref);
			});
		});
	});
}

template<typename vector_t, typename callback_t>
static void for_each_uniq(vector_t const &a, callback_t callback)
{
	for (count_t l = 0, r = 0; l < a.size(); l = r) {
		r = l;
		while (r < a.size() && a[l] == a[r])
			++r;
		callback(l, r);
	}
}

int main(int argc, char *argv[])
{
	std::cout << std::fixed << std::setprecision(2);

	log_level(log_level_t::debug);

	if (argc != 2) {
		log_error("geo-base-show") << "geo-base-show <geodata.dat>";
		return -1;
	}

	try {
		geo_base_t geo_base(argv[1]);
		geo_base.show(std::cout);

		geo_data_t const *dat = geo_base.geo_data();

		{
			std::vector<count_t> part_refs;
			for_each_polygon(dat, [&] (polygon_t const *polygon) {
				for_each_part(dat, polygon, [&] (part_t const *part) {
					part_refs.push_back((part + 1)->edge_refs_offset - part->edge_refs_offset);
				});
			});

			std::sort(part_refs.begin(), part_refs.end());
			count_t part_refs_memory = 0;

			for_each_uniq(part_refs, [&] (count_t l, count_t r) {
				for (count_t i = l; i < r; ++i)
					part_refs_memory += part_refs[i] * sizeof(i);

				double percent = r * 100.0 / part_refs.size();
				double memory = part_refs_memory / (1024. * 1024.0);

				log_info("geo-base-show", "refs count stat") << percent << "% <= " << part_refs[l] << " (" << memory << " MB)";
			});
		}

		{
			struct region_memory_t {
				region_id_t region_id;
				count_t memory;

				region_memory_t(std::pair<region_id_t, count_t> const &p)
					: region_id(p.first)
					, memory(p.second)
				{
				}
			};

			std::unordered_map<region_id_t, count_t> region_memory;
			for_each_polygon(dat, [&] (polygon_t const *polygon) {
				for_each_part(dat, polygon, [&] (part_t const *part) {
					region_memory[polygon->region_id] += ((part + 1)->edge_refs_offset - part->edge_refs_offset) * sizeof(ref_t);
				});
			});

			std::vector<region_memory_t> regions(region_memory.begin(), region_memory.end());
			std::sort(regions.begin(), regions.end(), [&] (region_memory_t const &a, region_memory_t const &b) {
				return a.memory > b.memory;
			});	

			static count_t const REGIONS_COUNT = 5;

			for (count_t i = 0; i < REGIONS_COUNT; ++i) {
				double memory = regions[i].memory / (1024. * 1024.);
				log_info("geo-base-show", "region memory", i + 1) << regions[i].region_id << " = " << memory << " MB";
			}
		}

		{
			double compressed_memory = 0;
			for_each_polygon(dat, [&] (polygon_t const *polygon) {	
				ref_t base_ref = dat->edges_count + 1;
				for_each_part(dat, polygon, [&] (part_t const *part) {
					for_each_ref(dat, part, [&] (ref_t const &r) {
						base_ref = std::min(base_ref, r);
					});
				});

				count_t bit = 1;
				for_each_part(dat, polygon, [&] (part_t const *part) {
					for_each_ref(dat, part, [&] (ref_t const &r) {
						ref_t x = r - base_ref;
						while (((1LLU << bit) - 1) < x)
							++bit;
					});
				});

				count_t count = 0;
				for_each_part(dat, polygon, [&] (part_t const *part) {
					for_each_ref(dat, part, [&] (ref_t const &) {
						++count;
					});
				});

				compressed_memory += 1.0 * count * bit / 8.0 + sizeof(ref_t) + 1.0;
			});

			log_info("geo-base-show") << "Polygon bits compressed memory = " << compressed_memory / (1024. * 1024.) << " MB";
		}

		{
			double compressed_memory = 0;
			for_each_polygon(dat, [&] (polygon_t const *polygon) {
				for_each_part(dat, polygon, [&] (part_t const *part) {
					ref_t base_ref = dat->edges_count + 1;
					for_each_ref(dat, part, [&] (ref_t const &r) {
						base_ref = std::min(base_ref, r);
					});

					count_t bit = 1;
					for_each_ref(dat, part, [&] (ref_t const &r) {
						ref_t x = r - base_ref;
						while ((1LLU << bit) - 1 < x)
							++bit;
					});	

					count_t count = 0;
					for_each_ref(dat, part, [&] (ref_t const &) {
						++count;
					});

					compressed_memory += 1.0 * count * bit / 8.0 + sizeof(ref_t) + 1.0;
				});
			});

			log_info("geo-base-show") << "Part bits compressed memory = " << compressed_memory / (1024. * 1024.) << " MB";
		}

	} catch (std::exception const &e) {
		log_error("geo-base-show", "EXCEPTION") << e.what();
	}

	return 0;
}
