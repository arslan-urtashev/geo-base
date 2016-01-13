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

#include <geo_base/util/block_allocator.h>

#include <new>

namespace geo_base {

static size_t const MEMORY_IS_USED_FLAG = ~0ull;
static size_t const SIZEOF_SIZE = align_memory(sizeof(size_t));

void *block_allocator_t::allocate(size_t count)
{
	count = align_memory(count);
	if (bytes_allocated_ + count + SIZEOF_SIZE > bytes_limit_)
		throw std::bad_alloc();
	char *begin = ((char *) data_) + bytes_allocated_;
	char *end = begin + count;
	*((size_t *) end) = MEMORY_IS_USED_FLAG;
	bytes_allocated_ += count + SIZEOF_SIZE;
	return begin;
}

static void relax_block(char *begin, size_t *count)
{
	while (*count > 0) {
		char *ptr = begin + *count - SIZEOF_SIZE;
		if (*((size_t *) ptr) == MEMORY_IS_USED_FLAG)
			return;
		*count -= *((size_t *) ptr) + SIZEOF_SIZE;
	}
}

void block_allocator_t::deallocate(void *ptr, size_t count)
{
	count = align_memory(count);
	char *begin = (char *) ptr;
	char *end = begin + count;
	if (*((size_t *) end) != MEMORY_IS_USED_FLAG)
		throw exception_t("Trying to deallocate not allocated pointer %p", ptr);
	*((size_t *) end) = count;
	relax_block((char *) data_, &bytes_allocated_);
}

} // namespace geo_base
