#pragma once

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace geo_base {

class fd_stat_t {
public:
	fd_stat_t(int fd)
	{
		if (fstat(fd, &st) < 0)
			throw exception_t("%s", strerror(errno));
	}

	size_t length() const
	{
		return st.st_size;
	}

private:
	struct stat st;
};

}
