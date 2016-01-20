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
#include <geo_base/util/exception.h>
#include <geo_base/util/memory.h>
#include <geo_base/util/common.h>

namespace geo_base {

class block_allocator_t : public allocator_t {
public:
	block_allocator_t()
		: data_(nullptr)
		, bytes_allocated_(0)
		, bytes_limit_(0)
	{
	}

	block_allocator_t(void *data, size_t bytes_limit)
		: data_(data)
		, bytes_allocated_(0)
		, bytes_limit_(bytes_limit)
	{
	}

	block_allocator_t(block_allocator_t &&a)
		: block_allocator_t()
	{
		std::swap(data_, a.data_);
		std::swap(bytes_allocated_, a.bytes_allocated_);
		std::swap(bytes_limit_, a.bytes_limit_);
	}

	block_allocator_t &operator = (block_allocator_t &&a)
	{
		std::swap(data_, a.data_);
		std::swap(bytes_allocated_, a.bytes_allocated_);
		std::swap(bytes_limit_, a.bytes_limit_);
		return *this;
	}

	void *allocate(size_t number) override;

	size_t allocate_size(size_t number) const;

	void deallocate(void *ptr, size_t number) override;

	size_t total_allocated_size() const
	{
		return bytes_allocated_;
	}

	void setup(void *data, size_t bytes_limit)
	{
		data_ = data;
		bytes_limit_ = bytes_limit;
		bytes_allocated_ = 0;
	}

private:
	void *data_;
	size_t bytes_allocated_;
	size_t bytes_limit_;
	
	GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(block_allocator_t);
};

} // namespace geo_base
