#include "geo_base.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("USAGE: geo-base-lookup <geodata.dat>\n");
		return -1;
	}

	geo_base_t geo_base;
	if (geo_base_init(argv[1], &geo_base) < 0) {
		printf("Can't init geo_base\n");
		return -1;
	}

	geo_location_t l;
	while (scanf("%lf %lf", &l.lat, &l.lon) == 2) {
		geo_region_id_t region_id = geo_base_lookup(&geo_base, &l);
		printf("%llu\n", (region_id == GEO_UNKNOWN_REGION_ID ? 0 : region_id));
	}
	
	geo_base_fini(&geo_base);

	return 0;
}
