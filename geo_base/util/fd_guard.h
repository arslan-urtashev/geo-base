// copyright (c) 2015 urtashev arslan. all rights reserved.
// contacts: <urtashev@gmail.com>
//
// permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "software"), to deal in the software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the software, and to permit persons to whom the
// software is furnished to do so, subject to the following conditions:
//
//   the above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the software.
//
// the software is provided "as is", without warranty of any kind, express or implied, including
// but not limited to the warranties of merchantability, fitness for a particular purpose and
// noninfringement. in no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract, tort or otherwise, arising from,
// out of or in connection with the software or the use or other dealings in the software.

#pragma once

#include <geo_base/util/algo.h>
#include <geo_base/util/common.h>
#include <geo_base/util/log.h>

#include <unistd.h>

namespace geo_base {

class fd_guard_t {
public:
	fd_guard_t()
		: fd_(-1)
	{
	}

	explicit fd_guard_t(int fd)
		: fd_(fd)
	{
		if (fd_ != -1)
			log_debug("Guard fd %d", fd_);
	}

	fd_guard_t(fd_guard_t&& g)
		: fd_(-1)
	{
		std::swap(fd_, g.fd_);
	}

	fd_guard_t &operator = (fd_guard_t&& g)
	{
		std::swap(fd_, g.fd_);
		return *this;
	}

	int fd() const
	{
		return fd_;
	}

	~fd_guard_t()
	{
		if (fd_ != -1) {
			log_debug("Close fd %d", fd_);
			close(fd_);
		}
	}

private:
	int fd_;

	GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(fd_guard_t);
};

} // namespace geo_base
