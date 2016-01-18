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

#include <geo_base/util/file.h>
#include <geo_base/util/mem_guard.h>
#include <geo_base/util/memory.h>

namespace geo_base {

class mem_file_t : public file_t {
public:
	static size_t const DEFAULT_MMAP_SIZE = 8_gb;

	mem_file_t()
		: mem_guard_()
	{
	}

	mem_file_t(mem_file_t &&f)
		: file_t(std::forward<file_t>(f))
	{
		std::swap(mem_guard_, f.mem_guard_);
	}

	mem_file_t &operator = (mem_file_t &&f)
	{
		file_t::operator = (std::forward<file_t>(f));
		std::swap(mem_guard_, f.mem_guard_);
		return *this;
	}

	void read_open(char const *path);

	void read_write_open(char const *path, size_t mmap_size = DEFAULT_MMAP_SIZE);

	void *data() const
	{
		return mem_guard_.data();
	}

	size_t size() const
	{
		return mem_guard_.size();
	}

private:
	mem_guard_t mem_guard_;

	mem_file_t(mem_file_t const &) = delete;
	mem_file_t &operator = (mem_file_t const &) = delete;
};

} // namespace geo_base
