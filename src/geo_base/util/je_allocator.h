// Copyright (c) 2016 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <geo_base/util/allocator.h>
#include <jemalloc/jemalloc.h>

namespace geo_base {

class je_allocator_t : public allocator_t {
public:
	void *allocate(size_t count) override
	{
		return yesmalloc(count);
	}

	void deallocate(void *ptr, size_t) override
	{
		yesfree(ptr);
	}
};

template<typename value_t>
class je_temp_alloc_t {
public:
	typedef value_t value_type;
	typedef value_t* pointer;
	typedef value_t& reference;
	typedef value_t const* const_pointer;
	typedef value_t const& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	template<typename other_t>
	struct rebind {
		typedef je_temp_alloc_t<other_t> other;
	};

	pointer allocate(size_type n)
	{
		return (pointer) allocator_.allocate(n * sizeof(value_type));
	}

	void deallocate(pointer p, size_type n)
	{
		allocator_.deallocate(p, n);
	}

private:
	je_allocator_t allocator_;
};

}
