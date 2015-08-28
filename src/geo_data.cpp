#include "geo_data.hpp"

#include "exception.hpp"
#include "io.hpp"

namespace geo_base {

void geo_data_map(void *dat, geo_data_t *geo_data)
{
#define TROLL_DEF_VAR(var_t, var)
	// undef

#define TROLL_DEF_PTR(ptr_t, ptr) \
	if (geo_data->ptr) \
		geo_data->ptr =	(ptr_t *) (((ptrdiff_t) dat) + ((char *) geo_data->ptr))

#define TROLL_DEF_ARR(arr_t, arr) \
	TROLL_DEF_PTR(arr_t, arr);

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_PTR
#undef TROLL_DEF_VAR
#undef TROLL_DEF_ARR

	if (geo_data->version != geo_data_version())
		throw exception_t("Wrong geo_data version, expect: %lu, got: %lu", geo_data_version(), geo_data->version);
}

void geo_data_save(void *dat, geo_data_t *geo_data)
{
	geo_data->version = geo_data_version();

#define TROLL_DEF_VAR(var_t, var)
	// undef

#define TROLL_DEF_PTR(ptr_t, ptr) \
	if (geo_data->ptr) \
		geo_data->ptr = (ptr_t *) (((char *) geo_data->ptr) - ((ptrdiff_t) dat));

#define TROLL_DEF_ARR(arr_t, arr) \
	TROLL_DEF_PTR(arr_t, arr);

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR
}

region_id_t geo_data_lookup(geo_data_t const &geo_data, location_t const &location)
{
	point_t point(location);

	ref_t box_x = (point.x - box_t::LOWER_X) / box_t::DELTA_X;
	ref_t box_y = (point.y - box_t::LOWER_Y) / box_t::DELTA_Y;
	ref_t box_ref = box_x * box_t::COUNT_Y + box_y;

	if (box_ref >= geo_data.boxes_count)
		return -1;

	count_t refs_offset = geo_data.boxes[box_ref].polygon_refs_offset;
	count_t refs_count = geo_data.boxes[box_ref].polygon_refs_count;

	polygon_t const *answer = nullptr;

	for (ref_t ref = refs_offset; ref < refs_offset + refs_count; ++ref) {
		ref_t i = geo_data.polygon_refs[ref];

		if (geo_data.polygons[i].contains(point, geo_data.parts, geo_data.edge_refs, geo_data.edges, geo_data.points)) {
			if (!answer || answer->square > geo_data.polygons[i].square) {
				answer = &(geo_data.polygons[i]);

			} else if (answer->square == geo_data.polygons[i].square) {
				region_t const *r1 = find(geo_data.regions, geo_data.regions_count, answer->region_id);
				region_t const *r2 = find(geo_data.regions, geo_data.regions_count, geo_data.polygons[i].region_id);
				if (r1->square > r2->square)
					answer = &(geo_data.polygons[i]);
			}
		}
	}

	return answer ? answer->region_id : -1;
}

void geo_data_show(geo_data_t const &geo_data, output_t &out)
{
	size_t total = 0;

	out << "geo_data:" << '\n';

#define TROLL_DEF_VAR(var_t, var) \
	out << "  " << #var << " = " << geo_data.var << '\n'; \
	total += sizeof(var_t);

#define TROLL_DEF_PTR(ptr_t, ptr) \
	// undef

#define TROLL_DEF_ARR(arr_t, arr) \
	out \
		<< "  " \
		<< #arr \
			<< " = "  \
		<< geo_data.arr##_count << " x " << sizeof(arr_t) \
			<< " = " \
		<< geo_data.arr##_count * sizeof(arr_t) \
			<< " = " \
		<< geo_data.arr##_count * sizeof(arr_t) / (1024. * 1024.) << " MB" << '\n' \
	; \
	total += sizeof(arr_t) * geo_data.arr##_count;

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

	out << "Total = " << total * 1.0 / (1024. * 1024. * 1024.) << " GB" << '\n';
}

static void update_version(char const *str, version_t *version)
{
	for (size_t i = 0; str[i]; ++i)
		(*version) ^= (((version_t) str[i]) << ((i % (sizeof(version_t) / sizeof(char))) * 8));
}

version_t geo_data_version()
{
	version_t version = 0;

#define TROLL_DEF_VAR(var_t, var) \
	update_version(#var_t, &version); \
	update_version(#var, &version);

#define TROLL_DEF_PTR(ptr_t, ptr) \
	update_version(#ptr_t, &version); \
	update_version(#ptr, &version);

#define TROLL_DEF_ARR(arr_t, arr) \
	TROLL_DEF_VAR(count_t, arr##_count); \
	TROLL_DEF_PTR(arr_t, arr);

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

	return version;
}

}
