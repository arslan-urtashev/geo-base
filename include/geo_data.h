#pragma once

#include "box.h"
#include "common.h"
#include "edge.h"
#include "io.h"
#include "kv.h"
#include "location.h"
#include "part.h"
#include "point.h"
#include "polygon.h"
#include "region.h"

namespace geo_base {

#define TROLL_DEF_VAR(var_t, var) \
	var_t var;

#define TROLL_DEF_PTR(ptr_t, ptr) \
	ptr_t *ptr;

#define TROLL_DEF_ARR(arr_t, arr) \
	TROLL_DEF_VAR(count_t, arr##_count); \
	TROLL_DEF_PTR(arr_t, arr);

#define TROLL_DEF_GEO_DATA \
	TROLL_DEF_VAR(version_t, version); \
	TROLL_DEF_ARR(point_t, points); \
	TROLL_DEF_ARR(edge_t, edges); \
	TROLL_DEF_ARR(ref_t, edge_refs); \
	TROLL_DEF_ARR(part_t, parts); \
	TROLL_DEF_ARR(polygon_t, polygons); \
	TROLL_DEF_ARR(ref_t, polygon_refs); \
	TROLL_DEF_ARR(box_t, boxes); \
	TROLL_DEF_ARR(char, blobs); \
	TROLL_DEF_ARR(kv_t, kvs); \
	TROLL_DEF_ARR(region_t, regions);

struct geo_data_t {
	TROLL_DEF_GEO_DATA
};

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

version_t geo_data_version();

void geo_data_map(void *dat, geo_data_t* geo_data);
void geo_data_save(void *dat, geo_data_t* geo_data);
void geo_data_show(geo_data_t const &geo_data, output_t &out);

region_id_t geo_data_lookup(geo_data_t const &geo_data, location_t const &location);

template<typename callback_t>
void geo_data_kv_each(geo_data_t const &geo_data, region_id_t region_id, callback_t callback)
{
	region_t const *region = find(geo_data.regions, geo_data.regions_count, region_id);
	while (region && region - geo_data.regions < geo_data.regions_count && region->region_id == region_id) {
		for (count_t i = region->kvs_offset; i < region->kvs_count + region->kvs_offset; ++i) {
			char const *k = (char const *) geo_data.blobs + geo_data.kvs[i].k;
			char const *v = (char const *) geo_data.blobs + geo_data.kvs[i].v;
			callback(k, v);
		}
		++region;
	}
}

}
