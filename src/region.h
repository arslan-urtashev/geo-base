#pragma once

#include "common.h"

namespace troll {

struct region_t {
	region_id_t region_id;
	offset_t kvs_offset;
	count_t kvs_count;

	bool operator == (region_t const &r) const
	{
		return region_id == r.region_id;
	}
};

}
