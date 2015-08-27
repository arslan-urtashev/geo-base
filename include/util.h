#pragma once

#include "common.h"
#include "exception.h"
#include "io.h"
#include "location.h"
#include "log.h"
#include "stop_watch.h"

#include <sstream>
#include <string>
#include <functional>
#include <vector>

#include <math.h>

namespace troll {

using read_txt_visitor_t = std::function<void (region_id_t, std::vector<location_t> const &, std::vector<std::string> const &blobs)>;
using process_locations_visitor_t = std::function<void (std::vector<location_t> const &)>;

void geo_read_txt(input_t &in, read_txt_visitor_t callback);
void process_locations(std::vector<location_t> const &raw_locations, std::vector<location_t> &locations,
	process_locations_visitor_t callback);


}
