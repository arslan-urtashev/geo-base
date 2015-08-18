#pragma once

#include "base_alloc.h"
#include "geo_data.h"

namespace troll {

class generate_t : public base_alloc_t {
public:
	generate_t(char const *path)
		: base_alloc_t (path)
	{
		geo_data = (geo_data_t *) alloc(sizeof(geo_data_t));
	}

	void save()
	{
		geo_data_save(addr(), geo_data);
	}

private:
	geo_data_t *geo_data;
};

}
