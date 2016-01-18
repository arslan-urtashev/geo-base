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

#include <geo_base/core/geo_data.h>
#include <geo_base/util/block_allocator.h>

namespace geo_base {

class geo_data_map_t : public geo_data_t {
#define GEO_BASE_DEF_VAR(var_t, var) \
public: \
	var_t const &var() const override \
	{ \
		return var##_; \
	} \
private: \
	var_t var##_;

#define GEO_BASE_DEF_ARR(arr_t, arr) \
public: \
	arr_t const *arr() const override \
	{ \
		return arr##_; \
	} \
	number_t arr##_number() const override \
	{ \
		return arr##_number_; \
	} \
private: \
	number_t arr##_number_; \
	arr_t const *arr##_;

	GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

public:
	geo_data_map_t();

	geo_data_map_t(geo_data_t const &data, block_allocator_t *allocator);

	geo_data_map_t(char const *data, size_t size)
		: geo_data_map_t()
	{
		data_ = data;
		size_ = size;
		remap();
	}

	geo_data_map_t(geo_data_map_t &&dat)
		: geo_data_map_t()
	{
		std::swap(data_, dat.data_);
		std::swap(size_, dat.size_);
		remap();
		dat.remap();
	}

	geo_data_map_t &operator =(geo_data_map_t &&dat)
	{
		std::swap(data_, dat.data_);
		std::swap(size_, dat.size_);
		remap();
		dat.remap();
		return *this;
	}

	char const *data() const
	{
		return data_;
	}
	
	size_t size() const
	{
		return size_;
	}

private:
	void remap();

	char const *data_;
	size_t size_;

	geo_data_map_t(geo_data_map_t const &) = delete;
	geo_data_map_t &operator = (geo_data_map_t const &) = delete;
};

} // namespace geo_base
