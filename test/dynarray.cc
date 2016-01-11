// copyright (c) 2015 urtashev arslan. all rights reserved.
// contacts: <urtashev@gmail.com>
//
// permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "software"), to deal in the software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the software, and to permit persons to whom the
// software is furnished to do so, subject to the following conditions:
//
//   the above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the software.
//
// the software is provided "as is", without warranty of any kind, express or implied, including
// but not limited to the warranties of merchantability, fitness for a particular purpose and
// noninfringement. in no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract, tort or otherwise, arising from,
// out of or in connection with the software or the use or other dealings in the software.

#include <geo_base/util/dynarray.h>
#include <geo_base/util/memory.h>
#include <geo_base/util/pool_allocator.h>
#include <test/geo_base_test.h>

#include <cassert>

using namespace geo_base;

struct dynarray_test_t : geo_base_test_t {
};

struct data_t {
	size_t *counter;
	size_t plus;

	data_t()
	{
		assert(false);
	}

	data_t(size_t *counter, size_t plus)
		: counter(counter)
		, plus(plus)
	{
		*counter += plus;
	}

	data_t &operator = (data_t const &d)
	{
		counter = d.counter;
		plus = d.plus;
		*counter += plus;
		return *this;
	}

	~data_t()
	{
		*counter -= plus;
	}
};

TEST_F(dynarray_test_t, destructor)
{
	pool_allocator_t allocator(1_kb);

	size_t counter = 13ull;

	{
		dynarray_t<data_t> arr(50, &allocator);

		for (size_t i = 0; i < 20; ++i)
			arr.push_back(data_t(&counter, i + 1));

		ASSERT_EQ(223ull, counter);
	}

	ASSERT_EQ(13ull, counter);
}
