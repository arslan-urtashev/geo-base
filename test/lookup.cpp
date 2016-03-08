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

#include <geo_base/core/geo_base.h>
#include <geo_base/core/geo_data/debug.h>
#include <geo_base/generator/generator.h>
#include <geo_base/generator/gen_geo_data.h>
#include <geo_base/library/memory.h>
#include <geo_base/library/pool_allocator.h>
#include <geo_base/open_street_map/converter.h>
#include <test/geo_base_test.h>

#include <utility>
#include <sstream>
#include <iomanip>

using namespace geo_base;

class lookup_test_t : public test_t {
};

TEST_F(lookup_test_t, raw_lookup_test)
{
    pool_allocator_t allocator(1_mb);

    generator::config_t config;
    config.save_raw_borders = true;

    generator::geo_data_test_t geo_data;
    generator::generator_t generator(config, &geo_data, &allocator);

    proto::region_t region;

    proto::polygon_t *polygon = region.add_polygons();
    polygon->set_polygon_id(123);

    using pair_t = std::pair<int, int>;
    using vector_t = std::vector<pair_t>;

    for (pair_t const &p : vector_t{{0, 0}, {10, 0}, {10, 0}, {10, 10}}) {
        proto::location_t *l = polygon->add_locations();
        l->set_lon(p.first);
        l->set_lat(p.second);
    }

    region.set_region_id(123);

    generator.init();
    generator.update(region);
    generator.fini();

    geo_base_t geo_base(geo_data);

    EXPECT_EQ(123ull, geo_base.raw_lookup(location_t(5, 5)));
    EXPECT_EQ(UNKNOWN_GEO_ID, geo_base.raw_lookup(location_t(-1, -1)));
    EXPECT_EQ(UNKNOWN_GEO_ID, geo_base.raw_lookup(location_t(0, 3)));
    EXPECT_EQ(123ull, geo_base.raw_lookup(location_t(4, 0)));
    EXPECT_EQ(123ull, geo_base.raw_lookup(location_t(5, 5)));
}

std::string to_string(std::vector<geo_id_t> const &a)
{
    std::stringstream out;
    out << "[";
    for (size_t i = 0; i < a.size(); ++i) {
        if (i > 0)
            out << ", ";
        out << a[i];
    }
    out << "]";
    return out.str();
}

std::string to_string(location_t const &l)
{
    std::stringstream out;
    out << std::fixed << std::setprecision(6);
    out << "(" << l.lat << ", " << l.lon << ")";
    return out.str();
}

TEST_F(lookup_test_t, b2b)
{
    ASSERT_NO_THROW(open_street_map::run_pool_convert("test/andorra-latest.osm.pbf", "andorra-latest.pbf", 1));

    generator::config_t config;
    config.save_raw_borders = true;

    ASSERT_NO_THROW(generator::generate("andorra-latest.pbf", "andorra-latest.dat", config));

    geo_base_t geo_base("andorra-latest.dat");

    geo_data_t const &geo_data = geo_base.geo_data();

    static size_t const X_GRID = 1000;
    static size_t const Y_GRID = 1000;

    geo_base_t::debug_t lookup_debug;
    geo_base_t::debug_t raw_lookup_debug;

    rectangle_t const box = rectangle_t(geo_data.points(), geo_data.points_number());

    for (size_t x = 0; x < X_GRID; ++x) {
        for (size_t y = 0; y < Y_GRID; ++y) {
            location_t location;

            location.lon = to_double(box.x1);
            location.lat = to_double(box.y1);

            location.lon += to_double(box.x2 - box.x1) * x / X_GRID;
            location.lat += to_double(box.y2 - box.y1) * y / Y_GRID;

            geo_id_t const lookup_id = geo_base.lookup(location, &lookup_debug);
            geo_id_t const raw_lookup_id = geo_base.raw_lookup(location, &raw_lookup_debug);

            EXPECT_EQ(raw_lookup_id, lookup_id)
                << to_string(location)
                    << " "
                << to_string(lookup_debug)
                    << " "
                << to_string(raw_lookup_debug);

            EXPECT_EQ(raw_lookup_debug.size(), lookup_debug.size());

            if (raw_lookup_debug.size() == lookup_debug.size())
                for (size_t i = 0; i < raw_lookup_debug.size(); ++i)
                    EXPECT_EQ(raw_lookup_debug[i], lookup_debug[i]);
        }
    }
}
