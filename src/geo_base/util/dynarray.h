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

#include "allocator.h"

namespace geo_base {

template<typename data_t>
class dynarray_t {
	static_assert(std::is_trivially_copyable<data_t>::value, "Data must be trivially copyable");

public:
	dynarray_t()
		: size_(0)
		, capacity_(0)
		, data_(nullptr)
		, allocator_(nullptr)
	{
	}

	dynarray_t(size_t capacity, allocator_t *allocator)
		: size_(0)
		, capacity_(capacity)
		, data_(nullptr)
		, allocator_(allocator)
	{
		data_ = (data_t *) allocator_->allocate(capacity * sizeof(data_t));
	}

	dynarray_t(size_t size, size_t capacity, allocator_t *allocator)
		: size_(size)
		, capacity_(capacity)
		, data_(nullptr)
		, allocator_(allocator)
	{
		data_ = (data_t *) allocator_->allocate(capacity * sizeof(data_t));
	}

	data_t const &operator [] (size_t index) const
	{
		return data_[index];
	}

	data_t &operator [] (size_t index)
	{
		return data_[index];
	}

	data_t *begin()
	{
		return data_;
	}

	data_t *end()
	{
		return data_ + size_;
	}

	data_t *data()
	{
		return data_;
	}

	size_t size() const
	{
		return size_;
	}

	void resize(size_t size)
	{
		size_ = size;
	}

	void clear()
	{
		size_ = 0;
	}

	void push_back(data_t const &data)
	{
		data_[size_++] = data;
	}

	~dynarray_t()
	{
		if (data_)
			allocator_->deallocate(data_, capacity_ * sizeof(data_t));
	}

	dynarray_t(dynarray_t &&a)
		: size_(0)
		, capacity_(0)
		, data_(nullptr)
		, allocator_(nullptr)
	{
		std::swap(size_, a.size_);
		std::swap(capacity_, a.capacity_);
		std::swap(data_, a.data_);
		std::swap(allocator_, a.allocator_);
	}

	dynarray_t &operator = (dynarray_t &&a)
	{
		std::swap(size_, a.size_);
		std::swap(capacity_, a.capacity_);
		std::swap(data_, a.data_);
		std::swap(allocator_, a.allocator_);
		return *this;
	}

private:
	size_t size_;
	size_t capacity_;
	data_t *data_;
	allocator_t *allocator_;

	dynarray_t(dynarray_t const &) = delete;
	dynarray_t &operator = (dynarray_t const &) = delete;
};

} // namespace geo_base
