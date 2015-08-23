#pragma once

#include "common.h"
#include "exception.h"
#include "io.h"
#include "kv.h"
#include "location.h"
#include "log.h"
#include "vector.h"
#include "watch.h"

#include <sstream>
#include <string>
#include <functional>

#include <math.h>

namespace troll {

using read_txt_visitor_t = std::function<void (region_id_t, vector_t<location_t> const &, vector_t<kv_t> const &kv)>;
using process_locations_visitor_t = std::function<void (vector_t<location_t> const &)>;

void geo_read_txt(input_t &in, read_txt_visitor_t callback);
void process_locations(vector_t<location_t> const &raw_locations, vector_t<location_t> &locations,
	process_locations_visitor_t callback);


}
