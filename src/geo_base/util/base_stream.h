// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
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

namespace geo_base {

// FIXME: Rewrite this terrible abstraction.
// base_output_stream_t is a wrapper on base_allocator_t for geo_data serialize. In unit tests
// allocator can be switch to other.
class base_output_stream_t : public output_stream_t {
public:
	explicit base_output_stream_t(allocator_t *allocator)
		: allocator_(allocator)
	{
	}

	bool write(char const *ptr, size_t count) override
	{
		char *dst = (char *) allocator_->allocate(count);
		memcpy(dst, ptr, count);
		return true;
	}

private:
	allocator_t *allocator_;
};

} // namespace geo_base
