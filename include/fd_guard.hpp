#pragma once

#include <unistd.h>

#include "log.hpp"

namespace geo_base {

struct fd_guard_t {
	int fd;

	fd_guard_t(int fd = -1)
		: fd(fd)
	{
		guard(fd);
	}

	void close()
	{
		if (fd != -1) {
			log_debug("fd_guard_t") << "close fd = " << fd;
			::close(fd);
			fd = -1;
		}
	}

	void guard(int fd_)
	{
		close();
		fd = fd_;
		if (fd != -1)
			log_debug("fd_guard_t") << "guard fd = " << fd;
	}

	~fd_guard_t()
	{
		close();
	}

	fd_guard_t(fd_guard_t const &) = delete;
	fd_guard_t& operator = (fd_guard_t const &) = delete;
};

}
