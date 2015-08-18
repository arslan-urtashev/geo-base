#pragma once

#include "fd_base.h"
#include "fd_stat.h"
#include "mmap_guard.h"

#include <sys/mman.h>

namespace troll {

class mem_base_t : public fd_base_t {
protected:
	static size_t const MAX_MEM_LEN = 8LU * (1LU << 30); // 8 GB

	void rd(char const *path)
	{
		fd_base_t::rd(path);

		fd_stat_t stat(fd());

		size_t length = stat.length();
		void *addr = mmap(nullptr, length, PROT_READ, MAP_SHARED, fd(), 0);
		if (addr == MAP_FAILED)
			throw exception_t(strerror(errno));

		mmap_guard = mmap_guard_t(addr, length);
	}

	void rdwr(char const *path)
	{
		fd_base_t::rdwr(path);

		size_t length = MAX_MEM_LEN;
		void *addr = mmap(nullptr, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd(), 0);
		if (addr == MAP_FAILED)
			throw exception_t(strerror(errno));

		mmap_guard = mmap_guard_t(addr, length);
	}

public:
	void *addr() const
	{
		return mmap_guard.addr;
	}

	size_t length() const
	{
		return mmap_guard.length;
	}

private:
	mmap_guard_t mmap_guard;
};

}
