#pragma once

#include "geo_data.hpp"
#include "io.hpp"
#include "location.hpp"
#include "mem_base.hpp"

namespace geo_base {

class geo_base_t : public mem_base_t {
public:
	typedef geo_debug_t debug_t;

	geo_base_t(char const *path)
	{
		rd(path);
		dat = *((geo_data_t *) addr());
		geo_data_map(addr(), &dat);
	}

	void show(output_t &out) const
	{
		geo_data_show(dat, out);
	}

	region_id_t lookup(location_t const &l, debug_t *debug = nullptr) const
	{
		return geo_data_lookup(dat, l, debug);
	}

	template<typename callback_t>
	void kv_each(region_id_t const &region_id, callback_t callback) const
	{
		geo_data_kv_each(dat, region_id, callback);
	}

	geo_data_t const *geo_data() const
	{
		return &dat;
	}

private:
	geo_data_t dat;

	geo_base_t(geo_base_t const &) = delete;
	geo_base_t& operator = (geo_base_t const &) = delete;
};

}
