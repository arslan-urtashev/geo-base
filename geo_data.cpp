#include "geo_data.h"

namespace troll {

void geo_data_map(void *dat, geo_data_t *geo_data)
{
#define TROLL_DEF_VAR(var_t, var)
	// undef

#define TROLL_DEF_PTR(ptr_t, ptr) \
	if (geo_data->ptr) \
		geo_data->ptr =	(ptr_t *) (((ptrdiff_t) dat) + ((byte_t *) geo_data->ptr))

#define TROLL_DEF_ARR(arr_t, arr) \
	TROLL_DEF_PTR(arr_t, arr);

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_PTR
#undef TROLL_DEF_VAR
#undef TROLL_DEF_ARR
}

void geo_data_save(void *dat, geo_data_t *geo_data)
{
#define TROLL_DEF_VAR(var_t, var)
	// undef

#define TROLL_DEF_PTR(ptr_t, ptr) \
	if (geo_data->ptr) \
		geo_data->ptr = (ptr_t *) (((byte_t *) geo_data->ptr) - ((ptrdiff_t) dat));

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

	for (index_t i = 0; i < geo_data.polygons_count; ++i)
		if (geo_data.polygons[i].contains(point, geo_data.parts, geo_data.edge_indexes, geo_data.edges, geo_data.points))
			return geo_data.polygons[i].region_id;

	return -1;
}

void geo_data_show(geo_data_t const &geo_data, output_t &out)
{
#define TROLL_DEF_VAR(var_t, var) \
	out << #var << " = " << geo_data.var << '\n';

#define TROLL_DEF_PTR(ptr_t, ptr) \
	// undef

#define TROLL_DEF_ARR(arr_t, arr) \
	out \
		<< #arr \
			<< " = "  \
		<< geo_data.arr##_count << " x " << sizeof(arr_t) \
			<< " = " \
		<< geo_data.arr##_count * sizeof(arr_t) / (1024. * 1024.) << " MB" << '\n' \
	;

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR
}

}
