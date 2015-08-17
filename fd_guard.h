#pragma once

#include <unistd.h>

namespace troll {

struct fd_guard_t {
	fd_guard_t(int fd)
		: fd(fd)
	{
	}

	~fd_guard_t()
	{
		close(fd);
	}
};

}
