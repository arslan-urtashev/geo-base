#pragma once

#include "common.h"

#include <math.h>

namespace geo_base {

struct location_t {
	double lon;
	double lat;

	location_t(double lon = 0, double lat = 0)
		: lon(lon)
		, lat(lat)
	{
	}

	location_t &operator += (location_t const &l)
	{
		lon += l.lon;
		lat += l.lat;
		return *this;
	}

	bool operator == (location_t const &l) const
	{
		return lon == l.lon && lat == l.lat;
	}
	
	bool operator != (location_t const &l) const
	{
		return !(*this == l);
	}

	double dist_to(location_t const &l) const
	{
		static double const EARTH_RADIUS = 6371000.0;

		double lat1 = convert_to_radian(lat);
		double lon1 = convert_to_radian(lon);
		double lat2 = convert_to_radian(l.lat);
		double lon2 = convert_to_radian(l.lon);

		return EARTH_RADIUS * acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon1 - lon2));
	}
};

}
