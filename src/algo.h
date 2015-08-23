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

struct equal_t {
	template<typename a_t, typename b_t>
	bool operator () (a_t const &a, b_t const &b) const
	{
		return a == b;
	}
};

template<typename iter_t, typename x_t, typename cmp_t = less_t>
iter_t lower_bound(iter_t beg, iter_t end, x_t const &x, cmp_t cmp = cmp_t())
{
	return std::lower_bound(beg, end, x, cmp);
}

template<typename x_t, typename y_t, typename cmp_t = less_t>
x_t lower_bound(x_t x, size_t count, y_t const &y, cmp_t cmp = cmp_t())
{
	return lower_bound(x, x + count, y, cmp);
}

template<typename x_t, typename y_t, typename cmp_t = less_t>
x_t find(x_t x, size_t count, y_t const &y, cmp_t cmp = cmp_t())
{
	x_t ret = lower_bound(x, count, y, cmp);
	return (ret >= x && ret < x + count && !cmp(*ret, y) && !cmp(y, *ret)) ? ret : nullptr;
}

// template<typename iter_t, typename x_t, typename cmp_t = less_t>
// bool binary_search(iter_t beg, iter_t end, x_t const &x, cmp_t cmp = cmp_t())
// {
//  	return std::binary_search(beg, end, x, cmp);
// }

template<typename val_t>
void copy_n(val_t const *src, size_t count, val_t *dst)
{
	memcpy(dst, src, sizeof(val_t) * count);
	// std::copy_n(src, count, dst); <- COREDUMPS in __memmove_ssse3
}

template<typename x_t, typename y_t>
auto min(x_t const &x, y_t const &y) -> decltype(x < y ? x : y)
{
	return x < y ? x : y;
}

template<typename x_t, typename y_t>
auto max(x_t const &x, y_t const &y) -> decltype(x < y ? y : x)
{
	return x < y ? y : x;
}

template<typename a_t, typename b_t>
void swap(a_t &a, b_t &b)
{
	std::swap(a, b);
}

// template<typename iter_t>
// void reverse(iter_t begin, iter_t end)
// {
// 	std::reverse(begin, end);
// }

}
