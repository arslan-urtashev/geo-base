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

#include <geo_base/system.h>
#include <util/allocator.h>
#include <util/exception.h>

#include <algorithm>

namespace geo_base {

template<typename data_t>
class dynarray_t {
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
		: size_(0)
		, capacity_(capacity)
		, data_(nullptr)
		, allocator_(allocator)
	{
		data_ = (data_t *) allocator_->allocate(capacity * sizeof(data_t));
		resize(size);
	}

	data_t const &operator [] (size_t index) const
	{
		if (index >= size_)
			throw exception_t("Index %lu out of bounds %lu", index, size_);
		return data_[index];
	}

	data_t const &back() const
	{
		return data_[size_ - 1];
	}

	data_t const &front() const
	{
		return data_[0];
	}

	data_t &operator [] (size_t index)
	{
		if (index >= size_)
			throw exception_t("Index %lu out of bounds %lu", index, size_);
		return data_[index];
	}

	data_t *begin()
	{
		return data_;
	}

	data_t const *begin() const
	{
		return data_;
	}

	data_t *end()
	{
		return data_ + size_;
	}

	data_t const *end() const
	{
		return data_ + size_;
	}

	data_t *data()
	{
		return data_;
	}

	data_t const *data() const
	{
		return data_;
	}

	size_t size() const
	{
		return size_;
	}

	void resize(size_t size)
	{
		resize_down(size);
		resize_up(size);
	}

	void clear()
	{
		resize(0);
	}

	bool empty() const
	{
		return size_ == 0;
	}

	void push_back(data_t const &data)
	{
#ifndef NDEBUG
		if (size_ == capacity_)
			throw exception_t("Out of bounds push_back");
#endif
		data_[size_++] = data;
	}

	void pop_back()
	{
#ifndef NDEBUG
		if (size_ == 0)
			throw exception_t("Out of bounds pop_back");
#endif
		size_--;
		data_[size_].~data_t();
	}

	~dynarray_t()
	{
		if (data_) {
			clear();
			allocator_->deallocate(data_, capacity_ * sizeof(data_t));
		}
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
	void resize_down(size_t size)
	{
		if (IS_TRIVIALLY_DESTRUCTIBLE(data_t)) {
			if (size_ > size)
				size_ = size;
		} else {
			while (size_ > size)
				pop_back();
		}
	}

	void resize_up(size_t size)
	{
		if (IS_TRIVIALLY_CONSTRUCTIBLE(data_t)) {
			if (size_ < size)
				size_ = size;
		} else {
			while (size_ < size)
				push_back(data_t());
		}
	}

	size_t size_;
	size_t capacity_;
	data_t *data_;
	allocator_t *allocator_;

	dynarray_t(dynarray_t const &) = delete;
	dynarray_t &operator = (dynarray_t const &) = delete;
};

} // namespace geo_base
