#include "util.h"
#include "blob.h"

namespace troll {

void geo_read_txt(input_t &in, read_txt_visitor_t callback)
{
	std::string curstr;
	std::stringstream strin;

	region_id_t region_id;
	count_t locations_count;
	count_t blobs_count;

	vector_t<location_t> locations;
	vector_t<blob_t> blobs;

	count_t lines_count = 0;
	watch_t watch;

	while (std::getline(in, curstr)) {
		++lines_count;
		count_t region_line = lines_count;

		strin.clear();
		strin << curstr;

		if (!(strin >> region_id))
			throw exception_t("Can't read region_id on %u: \"%s\"", lines_count, curstr.c_str());

		if (!(strin >> locations_count))
			throw exception_t("Can't read locations count on %u: \"%s\"", lines_count, curstr.c_str());

		if (!(strin >> blobs_count))
			throw exception_t("Can't read blobs data count on %u: \"%s\"", lines_count, curstr.c_str());
		
		locations.resize(locations_count);

		for (location_t &l : locations) {
			++lines_count;
			
			if (!std::getline(in, curstr))
				throw exception_t("Wrong locations count for %ld on %u", region_id, region_line);

			strin.clear();
			strin << curstr;

			if (!(strin >> l.lon >> l.lat))
				throw exception_t("Can't read locations on %u: \"%s\"", lines_count, curstr.c_str());
		}

		blobs.resize(blobs_count);
		for (blob_t &s : blobs) {
			++lines_count;

			if (!(std::getline(in, curstr)))
				throw exception_t("Wrong blobs count for %ld on %u", region_id, region_line);

			s.clear();
			s.append(curstr.data(), curstr.length());
		}


		callback(region_id, locations, blobs);
	}

	log_info("geo_read_txt") << "Processed lines count = " << lines_count;
	log_info("geo_read_txt") << "Generated for " << watch.total() / 60. << " minutes";
}

void process_locations(vector_t<location_t> const &raw_locations, vector_t<location_t> &locations,
	process_locations_visitor_t callback)
{
	locations.clear();
	for (location_t const &l : raw_locations) {
		if (locations.empty() || locations.back() != l)
			locations.push_back(l);

		if (locations.size() > 3 && locations.front() == locations.back()) {
			locations.pop_back();
			callback(locations);
			locations.clear();
		}
	}

	if (locations.size() > 2)
		callback(locations);
}

}
