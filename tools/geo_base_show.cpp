#include "geo_base.hpp"
#include "log.hpp"

#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

using namespace geo_base;

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

		count_t one_part_refs = 0;
		std::map<region_id_t, count_t> counter;

		for (count_t i = 0; i < dat->polygons_count; ++i) {
			count_t parts_offset = dat->polygons[i].parts_offset;
			count_t parts_count = dat->polygons[i + 1].parts_count;

			for (count_t j = parts_offset; j < parts_offset + parts_count; ++j) {
				part_t const *p = &(dat->parts[j]);
				count_t refs_count = ((p + 1)->edge_refs_offset - p->edge_refs_offset);
				counter[dat->polygons[i].region_id] += sizeof(ref_t) * refs_count;
				one_part_refs = std::max(one_part_refs, refs_count);
			}
		}
		
		std::vector<std::pair<region_id_t, count_t>> regions(counter.begin(), counter.end());
		std::sort(regions.begin(), regions.end(),
			[&] (std::pair<region_id_t, count_t> const &a, std::pair<region_id_t, count_t> const &b) {
				return a.second > b.second;
			}
		);

		log_info("geo-base-show") << "One part refs count = " << one_part_refs;

		static count_t const REGIONS_COUNT = 10;

		for (count_t i = 0; i < REGIONS_COUNT; ++i)
			log_info("geo-base-show", "memory", i + 1) << regions[i].first << " = " << regions[i].second / (1024. * 1024.);

	} catch (std::exception const &e) {
		log_error("geo-base-show", "EXCEPTION") << e.what();
	}

	return 0;
}
