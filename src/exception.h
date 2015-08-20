#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <exception>

namespace troll {

class exception_t : public std::exception {
public:
	static size_t const MESSAGE_LIMIT = 128;

	template<typename... args_t>
	exception_t(char const *fmt, args_t... args) noexcept
	{
		snprintf(message, MESSAGE_LIMIT, fmt, args...);
	}

	exception_t(exception_t const &e) noexcept
	{
		strcpy(message, e.message);
	}

	exception_t& operator = (exception_t const &e) noexcept
	{
		strcpy(message, e.message);
		return *this;
	}

	virtual char const *what() const noexcept
	{
		return message;
	}
	
private:
	char message[MESSAGE_LIMIT];
};

}
