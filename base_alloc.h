#pragma once

#include "mem_base.h"
#include "typedef.h"

#include <unistd.h>

namespace troll {

class base_alloc_t : public mem_base_t {
public:
	static size_t const ALIGN = 16;

	base_alloc_t(char const *path)
		: offset(0)
	{
		rdwr(path);
	}

	void *alloc(size_t count)
	{
		size_t new_offset = align(offset + count);
		if (ftruncate(fd(), new_offset) < 0)
			throw exception_t("%s", strerror(errno));
		void *memory = ((byte_t *) addr()) + offset;
		offset = new_offset;
		return memory;
	}

private:
	static size_t align(size_t x)
	{
		while (x % ALIGN > 0)
			++x;
		return x;
	}

	size_t offset;
};

}
