#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#else
#	pragma GCC visibility push(default)
#endif

typedef uint64_t geo_region_id_t;

#define GEO_UNKNOWN_REGION_ID ((geo_region_id_t) -1)

typedef struct {
	void *geo_base;
} geo_base_t;

int geo_base_init(char const *path, geo_base_t *geo_base);
int geo_base_fini(geo_base_t *geo_base);

typedef struct {
	double lon;
	double lat;
} geo_location_t;

geo_region_id_t geo_base_lookup(geo_base_t const *geo_base, geo_location_t const *l);

#ifdef __cplusplus
}
#else
#	pragma GCC visibility pop
#endif
