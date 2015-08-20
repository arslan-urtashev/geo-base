#pragma once

#include "io.h"

namespace troll {

class log_output_t {
public:
	template<typename val_t>
	log_output_t &operator << (val_t const &v)
	{
		out << v;
		return *this;
	}

protected:
	log_output_t(output_t &out)
		: out(out)
	{
	}

	~log_output_t()
	{
		out << std::endl;
	}

	output_t &out;
};

class log_color_t : public log_output_t {
protected:
	log_color_t(output_t &out, int color)
		: log_output_t(out)
		, color(color)
	{
		if (color != 0)
			(*this) << "\033[" << color << "m";
	}

	~log_color_t()
	{
		if (color != 0)
			(*this) << "\033[0m";
	}

private:
	int color;
};

class log_level_t : public log_color_t {
protected:
	log_level_t(output_t &out, int color, char const *level)
		: log_color_t(out, color)
	{
		(*this) << "[" << level << "] ";
	}
};

class log_error : public log_level_t {
public:
	log_error()
		: log_level_t(std::cerr, 31, "error")
	{
	}
};

class log_info : public log_level_t {
public:
	log_info()
		: log_level_t(std::cerr, 32, "info")
	{
	}
};

class log_warning : public log_level_t {
public:
	log_warning()
		: log_level_t(std::cerr, 33, "warning")
	{
	}
};

class log_debug : public log_level_t {
public:
	log_debug()
		: log_level_t(std::cerr, 37, "debug")
	{
	}
};

}
