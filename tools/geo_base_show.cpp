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
			count_t parts_count = dat->polygons[i].parts_count;

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

		std::unordered_map<uint64_t, uint64_t> uniq_parts;
		for (count_t i = 0; i + 1 < dat->parts_count; ++i) {
			count_t refs_offset = dat->parts[i].edge_refs_offset;
			count_t refs_count = dat->parts[i + 1].edge_refs_offset - refs_offset;
			uint64_t hash = poly_hash_t<337>()((char const *) (dat->edge_refs + refs_offset), sizeof(ref_t) * refs_count);
			uniq_parts[hash] = sizeof(ref_t) * refs_count;
		}

		size_t total_parts_memory = 0;
		for (auto const &p : uniq_parts)
			total_parts_memory += p.second;

		std::unordered_set<uint64_t> uniq_pairs;
		for (count_t i = 0; i + 1 < dat->parts_count; ++i) {
			count_t refs_offset = dat->parts[i].edge_refs_offset;
			count_t refs_count = dat->parts[i + 1].edge_refs_offset - refs_offset;
			if (refs_count % 2 != 0) {
				log_warning("geo-base-show") << "Wrong part!";
				continue;
			}
			for (ref_t j = refs_offset; j < refs_offset + refs_count; j += 2)
				uniq_pairs.insert(*((uint64_t const *) (dat->edge_refs + j)));
		}

		log_info("geo-base-show") << "One part refs count = " << one_part_refs;

		log_info("geo-base-show") << "Uniq parts count = " << uniq_parts.size();
		log_info("geo-base-show") << "Uniq parts memory = " << total_parts_memory / (1024. * 1024.) << " MB";

		log_info("geo-base-show") << "Uniq pairs count = " << uniq_pairs.size();
		log_info("geo-base-show") << "Uniq pairs memory = " << uniq_pairs.size() * sizeof(uint64_t) / (1024. * 1024.) << " MB";

		static count_t const REGIONS_COUNT = 5;

		for (count_t i = 0; i < REGIONS_COUNT; ++i)
			log_info("geo-base-show", "memory", i + 1) << regions[i].first << " = " << regions[i].second / (1024. * 1024.) << " MB";

	} catch (std::exception const &e) {
		log_error("geo-base-show", "EXCEPTION") << e.what();
	}

	return 0;
}
