#pragma once

#include <sys/mman.h>

namespace troll {

struct mmap_guard_t {
	void *addr;
	size_t length;

	mmap_guard_t(void *addr = nullptr, size_t length = 0)
		: addr(addr)
		, length(length)
	{
		guard(addr, length);
	}

	void guard(void *addr_, size_t length_)
	{
		munmap();
		addr = addr_;
		length = length_;
	}

	void munmap()
	{
		if (addr) {
			::munmap(addr, length);
			addr = nullptr;
			length = 0;
		}
	}

	~mmap_guard_t()
	{
		munmap();
	}

	mmap_guard_t(mmap_guard_t const &) = delete;
	mmap_guard_t &operator = (mmap_guard_t const &) = delete;
};

}
