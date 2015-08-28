#pragma once

#include "io.hpp"

namespace geo_base {

struct watch_t {
	int day;
	int month;
	int year;
	int hours;
	int minutes;
	int seconds;

	watch_t();
};

output_t &operator << (output_t &out, watch_t const &w);

}
