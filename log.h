#pragma once

#include <stdio.h>

namespace troll {

#define troll_log(level, fmt, ...) \
	do { \
		fprintf(stderr, "[%s] [%s] ", level, __func__); \
		fprintf(stderr, fmt, __VA_ARGS__); \
		fprintf(stderr, "\n"); \
		fflush(stderr); \
	} while (false);

#define troll_log_debug(fmt, ...) \
	troll_log("debug", fmt, __VA_ARGS__);

#define troll_log_error(fmt, ...) \
	troll_log("error", fmt, __VA_ARGS__);

#define troll_log_warning(fmt, ...) \
	troll_log("warning", fmt, __VA_ARGS__);

}
