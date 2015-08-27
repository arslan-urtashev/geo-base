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

typedef std::function<void (region_id_t, std::vector<location_t> const &, std::vector<std::string> const &blobs)> read_txt_visitor_t;
typedef std::function<void (std::vector<location_t> const &)> process_locations_visitor_t;

void geo_read_txt(input_t &in, read_txt_visitor_t callback);
void process_locations(std::vector<location_t> const &raw_locations, std::vector<location_t> &locations,
	process_locations_visitor_t callback);


}
