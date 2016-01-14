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

#include <geo_base/open_street_map/converter.h>
#include <geo_base/util/log.h>
#include <geo_base/util/memory.h>
#include <geo_base/util/pool_allocator.h>

using namespace geo_base;
using namespace open_street_map;

int main(int argc, char *argv[])
{
	log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

	if (argc != 2) {
		log_error("USAGE: grep-boundary-ways <planet-latest.osm.pbf>");
		return -1;
	}

	size_t threads_count = std::thread::hardware_concurrency();
	if (threads_count == 0)
		++threads_count;

	log_info("Threads count: %lu", threads_count);

	std::vector<pool_allocator_t> allocators;
	for (size_t i = 0; i < threads_count; ++i)
		allocators.emplace_back(128_mb);

	std::vector<grep_boundary_ways_t> greps;
	for (size_t i = 0; i < threads_count; ++i)
		greps.emplace_back(&allocators[i]);

	run_pool_parse(argv[1], greps);

	for (size_t i = 0; i < greps.size(); ++i)
		log_info("Parsed by %lu grep: %lu ways", i + 1, greps[i].ways().size());

	std::unordered_set<geo_id_t> all_ways;
	while (!greps.empty()) {
		all_ways.insert(greps.back().ways().begin(), greps.back().ways().end());
		greps.pop_back();
	}

	log_info("Total grep %lu different ways", all_ways.size());

	return 0;
}
