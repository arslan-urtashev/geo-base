#pragma once

#include <time.h>

namespace troll {

class stop_watch_t {
public:
	stop_watch_t()
		: point(clock())
	{
	}

	double checkpoint()
	{
		double ret = (clock() - point) * 1.0 / CLOCKS_PER_SEC;
		point = clock();
		return ret;
	}

private:
	clock_t point;
};

}
