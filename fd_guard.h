#pragma once

#include <unistd.h>

namespace troll {

struct fd_guard_t {
	int fd;

	fd_guard_t(int fd = -1)
		: fd(fd)
	{
	}

	~fd_guard_t()
	{
		if (fd != -1)
			close(fd);
	}
};

}
