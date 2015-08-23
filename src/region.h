#pragma once

#include "common.h"

namespace troll {

struct region_t {
	region_id_t region_id;
	offset_t kvs_offset;
	count_t kvs_count;
	square_t square;

	bool operator == (region_t const &r) const
	{
		return region_id == r.region_id;
	}

	bool operator < (region_t const &r) const
	{
		return region_id < r.region_id;
	}

	bool operator < (region_id_t const &r) const
	{
		return region_id < r;
	}

	friend bool operator < (region_id_t const &region_id, region_t const &r)
	{
		return region_id < r.region_id;
	}
};

}
