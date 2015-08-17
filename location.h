#pragma once

#include <math.h>

namespace troll {

struct location_t {
	double lon;
	double lat;

	location_t(double lon = 0, double lat = 0)
		: lon(lon)
		, lat(lat)
	{
	}

	double dist_to(location_t const &l) const
	{
		static double const EARTH_RADIUS = 6371000.0;

		return EARTH_RADIUS * acos(sin(lat) * sin(l.lat) + cos(lat) * cos(l.lat) * cos(lon - l.lon));
	}
};

}
