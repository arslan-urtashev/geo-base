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

#include <geo_base/util/base_allocator.h>
#include <geo_base/util/memory.h>

#include <errno.h>

namespace geo_base {

static size_t const BASE_MAX_SIZE = 8ull * (1ull << 30);

base_allocator_t::base_allocator_t(char const *path)
	: file_t(path, file_t::READ_WRITE)
	, off_(0)
	, mem_guard_()
{
	void *memory = mmap(nullptr, BASE_MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd(), 0);
	if (memory == MAP_FAILED)
		throw exception_t("Unable init binary base allocator: %s", strerror(errno));
	mem_guard_ = mem_guard_t(memory, BASE_MAX_SIZE);
}

void *base_allocator_t::allocate(size_t count)
{
	count = align_memory(count);
	
	if (ftruncate(fd(), off_ + count) < 0)
		throw exception_t("Unable ftrancate: %s", strerror(errno));

	off_ += count;
	
	return ((char *) mem_guard_.data()) + off_ - count;
}

} // namespace geo_base
