#pragma once

#include "io.h"
#include "watch.h"

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
	log_output_t(output_t &out, char begin, char end)
		: out(out)
		, begin(begin)
		, end(end)
	{
		out << begin;
	}

	~log_output_t()
	{
		out << end;
		out.flush();
	}

	output_t &out;
	char begin;
	char end;
};

class log_color_t : public log_output_t {
protected:
	log_color_t(output_t &out, int color1, char begin, char end)
		: log_output_t(out, begin, end)
		, color(color1)
	{
#ifdef TROLL_LOG_NO_COLOR
		color = 0;
#endif
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
	log_level_t(output_t &out, int color, char const *level, char begin, char end)
		: log_color_t(out, color, begin, end)
	{
		(*this) << "[" << level << "] ";
	}
};

class log_time_t : public log_level_t {
protected:
	log_time_t(output_t &out, int color, char const *level, char begin, char end)
		: log_level_t(out, color, level, begin, end)
	{
		(*this) << "[" << watch_t() << "] ";
	}
};

class log_context_t : public log_time_t {
protected:
	template<typename... args_t>
	log_context_t(output_t &out, int color, char const *level, char begin, char end, args_t... args)
		: log_time_t(out, color, level, begin, end)
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
		: log_context_t(std::cerr, 31, "error", '\r', '\n', args...)
	{
	}
};

class log_info : public log_context_t {
public:
	template<typename... args_t>
	log_info(args_t... args)
		: log_context_t(std::cerr, 32, "info", '\r', '\n', args...)
	{
	}
};

class log_warning : public log_context_t {
public:
	template<typename... args_t>
	log_warning(args_t... args)
		: log_context_t(std::cerr, 33, "warning", '\r', '\n', args...)
	{
	}
};

class log_debug : public log_context_t {
public:
	template<typename... args_t>
	log_debug(args_t... args)
		: log_context_t(std::cerr, 37, "debug", '\r', '\n', args...)
	{
	}
};

class status : public log_context_t {
public:
	template<typename... args_t>
	status(args_t... args)
		: log_context_t(std::cerr, 32, "info", '\r', ' ', args...)
	{
	}

	static void clear()
	{
		std::cerr << '\n';
	}
};

struct log_null {
	template<typename... args_t>
	log_null(args_t...)
	{
	}

	template<typename val_t>
	log_null &operator << (val_t const &)
	{
		return *this;
	}
};

}
