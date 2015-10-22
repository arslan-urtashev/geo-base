// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
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

#include "log.h"

#include <mutex>
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

static char const *t(char *buffer)
{
	struct tm time_info;
	time_t timestamp = time(&timestamp);
	localtime_r(&timestamp, &time_info);
#ifndef GEO_BASE_LOG_DATE_FORMAT
	snprintf(buffer, TIMESTAMP_LIMIT, "[%02d:%02d:%02d]",
		time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
#else
	snprintf(buffer, TIMESTAMP_LIMIT, "[%02d-%02d-%04d %02d:%02d:%02d]",
		time_info.tm_mday, time_info.tm_mon + 1, time_info.tm_year + 1900,
		time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
#endif
	return buffer;
}

void log_write(log_level_t level, char const *message)
{
	static char const *P[LOG_LEVEL_COUNT] = {
		"",                    // LOG_LEVEL_DISABLE
		"\r\033[31m[  error]", // LOG_LEVEL_ERROR
		"\r\033[33m[warning]", // LOG_LEVEL_WARNING
		"\r\033[32m[   info]", // LOG_LEVEL_INFO
		"\r\033[34m[ status]", // LOG_LEVEL_STATUS
		"\r\033[90m[  debug]", // LOG_LEVEL_DEBUG
	};

	static char const *S[LOG_LEVEL_COUNT] = {
		"",                    // LOG_LEVEL_DISABLE
		"\033[0m\n", // LOG_LEVEL_ERROR
		"\033[0m\n", // LOG_LEVEL_WARNING
		"\033[0m\n", // LOG_LEVEL_INFO
		"\033[0m",   // LOG_LEVEL_STATUS
		"\033[0m\n", // LOG_LEVEL_DEBUG
	};

	char buffer[LOG_MESSAGE_LIMIT], tbuffer[TIMESTAMP_LIMIT];
	// Ignore logger snprintf errors.
	snprintf(buffer, LOG_MESSAGE_LIMIT, "%s %s %s %s", P[level], t(tbuffer), message, S[level]);

	logger_t::inst().write(level, buffer);
}

}
