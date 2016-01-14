// Copyright (c) 2015,2016 Urtashev Arslan. All rights reserved.
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

#include <geo_base/util/log.h>

#include <mutex>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

namespace geo_base {

static size_t const TIMESTAMP_LIMIT = 32;

class logger_t {
public:
	static logger_t &inst()
	{
		static logger_t logger;
		return logger;
	}

	void setup(int fd, log_level_t level)
	{
		fd_ = fd;
		level_ = level;
	}

	void write(log_level_t level, char const *message)
	{
		if (level <= level_) {
			std::lock_guard<std::mutex> lock(lock_);
			::write(fd_, message, strlen(message)); // Ignore logger write errors.
		}
	}

	int fd() const
	{
		return fd_;
	}

private:
	logger_t()
		: fd_(-1)
		, level_(LOG_LEVEL_DISABLE)	  
	{
	}

	int fd_;
	log_level_t level_;
	std::mutex lock_;
};

void log_setup(int fd, log_level_t level)
{
	logger_t::inst().setup(fd, level);
}

int log_fd()
{
	return logger_t::inst().fd();
}

static char const *t(char *buffer)
{
	struct timeval time_val;
	gettimeofday(&time_val, nullptr);

	struct tm time_info;
	localtime_r(&time_val.tv_sec, &time_info);

	snprintf(buffer, TIMESTAMP_LIMIT, "%02d:%02d:%02d.%06d",
		time_info.tm_hour, time_info.tm_min, time_info.tm_sec, (int) time_val.tv_usec);

	return buffer;
}

void log_write(log_level_t level, char const *message)
{
	static char const *A[LOG_LEVEL_COUNT] = {
		"", // LOG_LEVEL_DISABLE
		"\033[90m", // LOG_LEVEL_ERROR
		"\033[90m", // LOG_LEVEL_WARNING
		"\033[90m", // LOG_LEVEL_INFO
		"\033[90m", // LOG_LEVEL_DEBUG
	};

	static char const *B[LOG_LEVEL_COUNT] = {
		"", // LOG_LEVEL_DISABLE
		"\033[31;1mError\033[0m", // LOG_LEVEL_ERROR
		"\033[33;1mWarn\033[0m", // LOG_LEVEL_WARNING
		"\033[32;1mInfo\033[0m", // LOG_LEVEL_INFO
		"Debug", // LOG_LEVEL_DEBUG
	};

	static char const *C[LOG_LEVEL_COUNT] = {
		"", // LOG_LEVEL_DISABLE
		"\n", // LOG_LEVEL_ERROR
		"\n", // LOG_LEVEL_WARNING
		"\n", // LOG_LEVEL_INFO
		"\033[0m\n", // LOG_LEVEL_DEBUG
	};

	char buffer[LOG_MESSAGE_LIMIT], tbuffer[TIMESTAMP_LIMIT];
	// Ignore logger snprintf errors.
	snprintf(buffer, LOG_MESSAGE_LIMIT, "%s(%s) %s: %s%s",
		A[level], t(tbuffer), B[level], message, C[level]);

	logger_t::inst().write(level, buffer);
}

}
