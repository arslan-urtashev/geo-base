#include "watch.h"

namespace troll {

watch_t::watch_t()
{
	time_t t;
	time(&t);
	struct tm tm;
	localtime_r(&t, &tm);

	day = tm.tm_mday;
	month = tm.tm_mon;
	year = tm.tm_year;
	hours = tm.tm_hour;
	minutes = tm.tm_min;
	seconds = tm.tm_sec;

	month += 1;
	year += 1900;
}

static output_t &check_output(output_t &out, int x)
{
	if (x < 10)
		out << '0';
	out << x;
	return out;
}

output_t &operator << (output_t &out, watch_t const &w)
{
	check_output(out, w.day) << "-";
	check_output(out, w.month) << "-";
	out << w.year << " ";
	check_output(out, w.hours) << ":";
	check_output(out, w.minutes) << ":";
	check_output(out, w.seconds);
	return out;
}

}
