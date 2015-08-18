#pragma once

#include "geo_data.h"
#include "location.h"
#include "mem_base.h"

namespace troll {

class geo_base_t : public mem_base_t {
public:
	geo_base_t(char const *path)
	{
		rd(path);
		geo_data = (geo_data_t *) addr();
		geo_data_map(addr(), geo_data);
	}

	region_id_t lookup(location_t const &l) const
	{
		return geo_data_lookup(*geo_data, l);
	}

private:
	geo_data_t *geo_data;
};

}
