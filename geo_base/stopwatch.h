#pragma once

#include <time.h>

namespace troll {

class stopwatch_t {
public:
	stopwatch_t()
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
