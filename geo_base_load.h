#pragma once

#include "geo_data.h"
#include "mem_base.h"

namespace troll {

class geo_base_load_t : public mem_base_t {
public:
	geo_base_load_t(char const *path)
	{
		rdwr(path);
	}
};

}
