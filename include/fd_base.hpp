#pragma once

#include "exception.hpp"
#include "fd_guard.hpp"

#include <sys/file.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

namespace geo_base {

class fd_base_t {
protected:
	void rd(char const *path)
	{
		int fd = open(path, O_RDONLY | O_CLOEXEC | O_NOATIME);
		if (fd < 0)
			throw exception_t("%s", strerror(errno));
		fd_guard.guard(fd);
	}

	void rdwr(char const *path)
	{
		int fd = open(path, O_RDWR | O_CREAT | O_CLOEXEC | O_TRUNC, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
		if (fd < 0)
			throw exception_t("%s", strerror(errno));
		fd_guard.guard(fd);
	}

public:
	int fd() const
	{
		return fd_guard.fd;
	}

private:
	fd_guard_t fd_guard;
};

}
