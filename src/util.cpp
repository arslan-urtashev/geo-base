#include "algo.h"
#include "blob.h"
#include "point.h"
#include "util.h"

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

		if (sscanf(curstr.c_str(), "%lu %u %u", &region_id, &locations_count, &blobs_count) != 3)
			throw exception_t("Can't read counts on %u: \"%s\"", lines_count, curstr.c_str());

		locations.resize(locations_count);
		for (ref_t i = 0; i < locations_count; ++i) {
			++lines_count;
			
			if (!std::getline(in, curstr))
				throw exception_t("Wrong locations count for %ld on %u", region_id, region_line);

			if (sscanf(curstr.c_str(), "%lf %lf", &locations[i].lon, &locations[i].lat) != 2)
				throw exception_t("Can't read locations on %u: \"%s\"", lines_count, curstr.c_str());
		}

		blobs.resize(blobs_count);
		for (ref_t i = 0; i < blobs_count; ++i) {
			++lines_count;

			if (!(std::getline(in, curstr)))
				throw exception_t("Wrong blobs count for %ld on %u", region_id, region_line);

			blobs[i].clear();
			blobs[i].append(curstr.data(), curstr.length());
		}

		if (blobs.size() % 2 != 0)
			throw exception_t("Wrong kv count for %ld on %u", region_id, region_line);

		callback(region_id, locations, blobs);
	}

	log_info("geo_read_txt") << "Processed lines count = " << lines_count;
	log_info("geo_read_txt") << "Generated for " << watch.total() / 60. << " minutes";
}

#ifdef TROLL_CHECK_POINTS_ON_ONE_LINE
static bool last_points_on_one_line(vector_t<location_t> const &l)
{
	if (l.size() < 3)
		return false;
	point_t a(l[l.size() - 3]);
	point_t b(l[l.size() - 2]);
	point_t c(l[l.size() - 1]);
	return (c - a).cross(c - b) == 0;
}
#endif

void process_locations(vector_t<location_t> const &raw_locations, vector_t<location_t> &locations,
	process_locations_visitor_t callback)
{
	locations.clear();
	for (location_t const &l : raw_locations) {
		if (locations.empty() || locations.back() != l)
			locations.push_back(l);

#ifdef TROLL_CHECK_POINTS_ON_ONE_LINE
		if (last_points_on_one_line(locations)) {
			swap(locations[locations.size() - 2], locations[locations.size() - 1]);
			locations.pop_back();
		}
#endif

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
