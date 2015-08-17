#pragma once

#include <algorithm>

namespace troll {

struct less_t {
	template<typename a_t, typename b_t>
	bool operator () (a_t const &a, b_t const &b) const
	{
		return a < b;
	}
};

template<typename x_t, typename y_t, typename cmp_t = less_t>
x_t lower_bound(x_t x, size_t count, y_t const &y, cmp_t cmp = cmp_t())
{
	return std::lower_bound(x, x + count, y, cmp);
}

}
