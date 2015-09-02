#include "io.hpp"

#include "exception.hpp"

#include <unistd.h>

namespace geo_base {

bool gb_read(int fd, char *ptr, size_t count)
{
	while (count > 0) {
		ssize_t ret = read(fd, ptr, count);
		if (ret < 0)
			throw exception_t("%s", strerror(errno));
		if (ret == 0)
			return false;
		ptr += ret;
		count -= ret;
	}
	return true;
}

bool gb_write(int fd, char const *ptr, size_t count)
{
	while (count > 0) {
		ssize_t ret = write(fd, ptr, count);
		if (ret <= 0)
			throw exception_t("%s", strerror(errno));
		ptr += ret;
		count -= ret;
	}
	return true;
}

}
