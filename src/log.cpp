#include "log.h"

namespace troll {

static log_level_t level = log_level_t::null;
static bool color = true;

log_level_t log_level()
{
	return level;
}

log_level_t log_level(log_level_t l)
{
	return level = l;
}

bool log_color()
{
	return color;
}

bool log_color(bool b)
{
	return color = b;
}

}
