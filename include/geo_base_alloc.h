#pragma once

#include "base_alloc.h"
#include "geo_data.h"

namespace troll {

class geo_base_alloc_t : public base_alloc_t {
public:
	geo_base_alloc_t(char const *path)
		: base_alloc_t(path)
	{
		data = (geo_data_t *) alloc(sizeof(geo_data_t));
	}

	geo_data_t *geo_data() const
	{
		return data;
	}

	void show(output_t &out) const
	{
		geo_data_show(*data, out);
	}

	~geo_base_alloc_t()
	{
		geo_data_save(addr(), data);
	}

private:
	geo_data_t *data;
};

}
