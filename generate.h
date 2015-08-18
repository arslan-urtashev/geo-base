#pragma once

#include "base_alloc.h"
#include "geo_data.h"

namespace troll {

class generate_t {
public:
	generate_t(char const *path)
		: base(path)
	{
		geo_data = (geo_data_t *) base.alloc(sizeof(geo_data_t));
	}

private:
	geo_data_t *geo_data;
	base_alloc_t base;
};

}
