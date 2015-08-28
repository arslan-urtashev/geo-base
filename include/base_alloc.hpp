#pragma once

#include "log.hpp"
#include "mem_base.hpp"
#include "common.hpp"

#include <unistd.h>

namespace geo_base {

class base_alloc_t : public mem_base_t {
public:
	static size_t const ALIGN = 64;

	base_alloc_t(char const *path)
		: offset(0)
	{
		rdwr(path);

		if ((((ptrdiff_t) addr()) % ALIGN) != 0)
			log_warning("base_alloc") << "Mapped memory not aligned!";
	}

	void *alloc(size_t count)
	{
		size_t new_offset = align(offset + count);
		if (ftruncate(fd(), new_offset) < 0)
			throw exception_t("%s", strerror(errno));
		void *memory = ((char *) addr()) + offset;
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
