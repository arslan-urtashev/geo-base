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

#pragma once

#include <geo_base/library/exception.h>

#include <cstdio>

namespace geo_base {

size_t const LOG_MESSAGE_LIMIT = 1024;

enum log_level_t {
    LOG_LEVEL_DISABLE = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_COUNT
};

// Init logger. Setup fd output and logger level.
void log_setup(int fd, log_level_t level);

// Write log message with colors, level and current time.
// Example:
//   (13:24:11.123456) Info: Good job!
//   (13:24:11.323456) Warn: Ooops :(
//   (13:24:22.456789) Error: Hello, world!
void log_write(log_level_t level, char const *message);

// Log output file descriptor.
int log_fd();

// Current log level.
log_level_t log_level();

template<typename... args_t>
void log_write(log_level_t level, char const *fmt, args_t... args)
{
    if (level <= log_level()) {
        char buffer[LOG_MESSAGE_LIMIT];
        // Ignore logger snprintf errors.
        snprintf(buffer, LOG_MESSAGE_LIMIT, fmt, std::forward<args_t>(args)...);
        log_write(level, buffer);
    }
}

template<typename... args_t>
void log_error(args_t... args)
{
    log_write(LOG_LEVEL_ERROR, std::forward<args_t>(args)...);
}

template<typename... args_t>
void log_warning(args_t... args)
{
    log_write(LOG_LEVEL_WARNING, std::forward<args_t>(args)...);
}

template<typename... args_t>
void log_info(args_t... args)
{
    log_write(LOG_LEVEL_INFO, std::forward<args_t>(args)...);
}

template<typename... args_t>
void log_debug(args_t... args)
{
    log_write(LOG_LEVEL_DEBUG, std::forward<args_t>(args)...);
}

}
