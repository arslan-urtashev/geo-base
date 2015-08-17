#pragma once

#include "edge.h"
#include "part.h"
#include "point.h"
#include "polygon.h"
#include "region.h"
#include "typedef.h"

namespace troll {

#define TROLL_DEF_VAR(var_t, var) \
	var_t var;

#define TROLL_DEF_PTR(ptr_t, ptr) \
	ptr_t *ptr;

#define TROLL_DEF_ARR(arr_t, arr) \
	TROLL_DEF_VAR(count_t, arr##_count); \
	TROLL_DEF_PTR(arr_t, arr);

#define TROLL_DEF_GEO_BASE \
	TROLL_DEF_ARR(point_t, points); \
	TROLL_DEF_ARR(edge_t, edges); \
	TROLL_DEF_ARR(index_t, edge_indexes); \
	TROLL_DEF_ARR(polygon_t, polygons); \
	TROLL_DEF_ARR(region_t, regions);

struct geo_base_t {
	TROLL_DEF_GEO_BASE
};

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

void geo_base_map(void *dat, geo_base_t* geo_base);
void geo_base_save(void *dat, geo_base_t* geo_base);

}
