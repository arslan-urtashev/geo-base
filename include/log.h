#pragma once

#include "io.h"
#include "watch.h"

namespace troll {

enum class log_level_t {
	null = -1,
	error,
	warning,
	info,
	debug
};

log_level_t log_level();
log_level_t log_level(log_level_t level);

bool log_color();
bool log_color(bool b);

class log_output_t {
public:
	template<typename val_t>
	log_output_t &operator << (val_t const &v)
	{
		if (static_cast<int>(level) <= static_cast<int>(log_level()))
			out << v;
		return *this;
	}

protected:
	log_output_t(output_t &out, char begin, char end, log_level_t level)
		: out(out)
		, begin(begin)
		, end(end)
		, level(level)
	{
		(*this) << begin;
	}

	~log_output_t()
	{
		(*this) << end;
		out.flush();
	}

	output_t &out;
	char begin;
	char end;
	log_level_t level;
};

class log_color_t : public log_output_t {
protected:
	log_color_t(output_t &out, int color1, char begin, char end, log_level_t level)
		: log_output_t(out, begin, end, level)
		, color(color1)
	{
		if (log_color())
			(*this) << "\033[" << color << "m";
	}

	~log_color_t()
	{
		if (log_color())
			(*this) << "\033[0m";
	}

private:
	int color;
};

class log_lvl_t : public log_color_t {
protected:
	log_lvl_t(output_t &out, int color, char begin, char end, log_level_t level)
		: log_color_t(out, color, begin, end, level)
	{
		static char const *lvls[] = {
			"[  error]",
			"[warning]",
			"[   info]",
			"[  debug]"
		};

		if (level != log_level_t::null) {
			uint8_t lvl = static_cast<uint8_t>(level);
			if (lvl < sizeof(lvls))
				(*this) << lvls[lvl] << " ";
		}
	}
};

class log_time_t : public log_lvl_t {
protected:
	log_time_t(output_t &out, int color, char begin, char end, log_level_t level)
		: log_lvl_t(out, color, begin, end, level)
	{
		(*this) << "[" << watch_t() << "] ";
	}
};

class log_context_t : public log_time_t {
protected:
	template<typename... args_t>
	log_context_t(output_t &out, int color, char begin, char end, log_level_t level, args_t... args)
		: log_time_t(out, color, begin, end, level)
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
		: log_context_t(std::cerr, 31, '\r', '\n', log_level_t::error, args...)
	{
	}
};

class log_info : public log_context_t {
public:
	template<typename... args_t>
	log_info(args_t... args)
		: log_context_t(std::cerr, 32, '\r', '\n', log_level_t::info, args...)
	{
	}
};

class log_warning : public log_context_t {
public:
	template<typename... args_t>
	log_warning(args_t... args)
		: log_context_t(std::cerr, 33, '\r', '\n', log_level_t::warning, args...)
	{
	}
};

class log_debug : public log_context_t {
public:
	template<typename... args_t>
	log_debug(args_t... args)
		: log_context_t(std::cerr, 37, '\r', '\n', log_level_t::debug, args...)
	{
	}
};

class status : public log_context_t {
public:
	template<typename... args_t>
	status(args_t... args)
		: log_context_t(std::cerr, 32, '\r', ' ', log_level_t::info, args...)
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
