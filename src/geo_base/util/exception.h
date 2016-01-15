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

#pragma once

#include <cstdio>
#include <cstring>
#include <exception>
#include <utility>

namespace geo_base {

class exception_t : public std::exception {
public:
	static size_t const MESSAGE_LIMIT = 128;

	template<typename... args_t>
	exception_t(char const *fmt, args_t... args) noexcept
	{
		snprintf(buffer_, MESSAGE_LIMIT, fmt, std::forward<args_t>(args)...);
	}

	exception_t(char const *message) noexcept
	{
		strncpy(buffer_, message, MESSAGE_LIMIT);
	}

	char const *what() const noexcept override
	{
		return buffer_;
	}

private:
	char buffer_[MESSAGE_LIMIT];
};

#define THROW_CHECK(function) \
	do { \
		try { \
			function; \
		} catch (std::exception const &e) { \
			log_error("Thrown exception: %s", e.what()); \
			throw; \
		} catch (...) { \
			log_error("Thrown unknown exception"); \
			throw; \
		} \
	} while (false);

}
