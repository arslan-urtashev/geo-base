#include "geo_base.h"

#include "geo_base.hpp"

int geo_base_init(char const *path, geo_base_t *geo_base)
{
	try {
		geo_base->geo_base = reinterpret_cast<void*>(new geo_base::geo_base_t(path));
		return 0;

	} catch (...) {
		return -1;
	}
}

int geo_base_fini(geo_base_t *geo_base)
{
	if (geo_base->geo_base)
		delete reinterpret_cast<geo_base::geo_base_t*>(geo_base->geo_base);
	geo_base->geo_base = NULL;
	return 0;
}

geo_region_id_t geo_base_lookup(geo_base_t const *geo_base, geo_location_t const *l)
{
	geo_base::location_t location(l->lon, l->lat);
	return reinterpret_cast<geo_base::geo_base_t const *>(geo_base->geo_base)->lookup(location);
}
