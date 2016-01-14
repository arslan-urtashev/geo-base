// Copyright (c) 2016 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <fcntl.h>
#include <geo_base/util/fd_guard.h>
#include <gmock/gmock.h>
#include <unistd.h>

using namespace geo_base;

static char const *FILE_NAME = "fd_guard_test.txt";

static int read_write_open_fd(char const *path)
{
	return open(path, O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
}

TEST(fd_guard_t, close)
{
	int fd = read_write_open_fd(FILE_NAME);
	ASSERT_EQ(0, fcntl(fd, F_GETFD));

	{
		fd_guard_t guard(fd);
		ASSERT_EQ(0, fcntl(guard.fd(), F_GETFD));
		ASSERT_EQ(fd, guard.fd());
	}

	ASSERT_NE(0, fcntl(fd, F_GETFD));

	remove(FILE_NAME);
}

TEST(fd_guard_t, move_asign)
{
	int fd = read_write_open_fd(FILE_NAME);
	ASSERT_EQ(0, fcntl(fd, F_GETFD));

	{
		fd_guard_t guard0;

		{
			fd_guard_t guard(fd);
			ASSERT_EQ(0, fcntl(guard.fd(), F_GETFD));
			ASSERT_EQ(fd, guard.fd());

			guard0 = std::move(guard);
		}

		ASSERT_EQ(0, fcntl(guard0.fd(), F_GETFD));
		ASSERT_EQ(fd, guard0.fd());
	}

	ASSERT_NE(0, fcntl(fd, F_GETFD));

	remove(FILE_NAME);
}

TEST(fd_guard_t, move_construct)
{
	int fd = read_write_open_fd(FILE_NAME);
	ASSERT_EQ(0, fcntl(fd, F_GETFD));

	{
		fd_guard_t guard(fd);

		ASSERT_EQ(0, fcntl(guard.fd(), F_GETFD));
		ASSERT_EQ(fd, guard.fd());

		fd_guard_t guard1(std::move(guard));

		ASSERT_EQ(-1, guard.fd());
		ASSERT_EQ(0, fcntl(guard1.fd(), F_GETFD));
		ASSERT_EQ(fd, guard1.fd());
	}

	ASSERT_NE(0, fcntl(fd, F_GETFD));

	remove(FILE_NAME);
}
