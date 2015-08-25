#pragma once

#include <time.h>

namespace troll {

class stopwatch_t {
public:
	stopwatch_t()
		: start(clock())
		, point(clock())
	{
	}

	double checkpoint()
	{
		double ret = (clock() - point) * 1.0 / CLOCKS_PER_SEC;
		point = clock();
		return ret;
	}

	double total()
	{
		double ret = (clock() - start) * 1.0 / CLOCKS_PER_SEC;
		return ret;
	}

private:
	clock_t start;
	clock_t point;
};

}
