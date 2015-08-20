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

class log_context_t : public log_level_t {
protected:
	template<typename... args_t>
	log_context_t(output_t &out, int color, char const *level, args_t... args)
		: log_level_t(out, color, level)
	{
		context(args...);
	}

private:
	void context()
	{
	}

	template<typename arg_t, typename... args_t>
	void context(arg_t const &arg, args_t... args)
	{
		(*this) << "[" << arg << "] ";
		context(args...);
	}
};

class log_error : public log_context_t {
public:
	template<typename... args_t>
	log_error(args_t... args)
		: log_context_t(std::cerr, 31, "error", args...)
	{
	}
};

class log_info : public log_context_t {
public:
	template<typename... args_t>
	log_info(args_t... args)
		: log_context_t(std::cerr, 32, "info", args...)
	{
	}
};

class log_warning : public log_context_t {
public:
	template<typename... args_t>
	log_warning(args_t... args)
		: log_context_t(std::cerr, 33, "warning", args...)
	{
	}
};

class log_debug : public log_context_t {
public:
	template<typename... args_t>
	log_debug(args_t... args)
		: log_context_t(std::cerr, 37, "debug", args...)
	{
	}
};

}
