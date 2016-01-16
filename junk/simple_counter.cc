// Copyright (c) 2016 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <geo_base/open_street_map/simple_counter.h>
#include <geo_base/util/log.h>
#include <geo_base/util/memory.h>
#include <geo_base/util/pool_allocator.h>

using namespace geo_base;
using namespace open_street_map;

int main(int argc, char *argv[])
{
	log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

	if (argc != 2) {
		log_error("USAGE: simple-counter <planet-latest.osm.pbf>");
		return -1;
	}

	size_t const threads_count = optimal_threads_number();
	log_info("Threads count: %lu", threads_count);

	std::vector<pool_allocator_t> allocators;
	for (size_t i = 0; i < threads_count; ++i)
		allocators.emplace_back(128_mb);

	std::vector<simple_counter_t> counters;
	for (size_t i = 0; i < threads_count; ++i)
		counters.emplace_back(&allocators[i]);

	run_pool_parse(argv[1], counters);

	for (size_t i = 0; i < counters.size(); ++i)
		log_info("Parsed by %lu counter: %lu nodes, %lu ways, %lu relations (%lu blocks)", i + 1,
			counters[i].nodes_number(), counters[i].ways_number(),
			counters[i].relations_number(), counters[i].blocks_processed());

	return 0;
}
