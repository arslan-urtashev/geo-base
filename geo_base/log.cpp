#include "log.h"

namespace troll {

static log_level_t level = log_level_t::null;

log_level_t log_level()
{
	return level;
}

log_level_t log_level(log_level_t l)
{
	return level = l;
}

}
