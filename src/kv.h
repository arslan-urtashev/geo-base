#pragma once

#include "strbuf.h"

namespace troll {

struct kv_t {
	strbuf_t k;
	strbuf_t v;
};

}
