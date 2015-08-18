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
	}

	~mmap_guard_t()
	{
		if (addr)
			munmap(addr, length);
	}
};

}
