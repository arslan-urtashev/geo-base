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

#include <geo_base/generator/common.h>
#include <geo_base/generator/locations_converter.h>
#include <geo_base/generator/points_converter.h>
#include <geo_base/generator/raw_borders_handler.h>
#include <geo_base/library/log.h>

namespace geo_base {
namespace generator {

void raw_borders_handler_t::init()
{
    if (!config_.save_raw_borders)
        return;
}

void raw_borders_handler_t::update(geo_id_t region_id, geo_id_t polygon_id,
    dynarray_t<point_t> const &raw_points, raw_border_t::type_t type)
{
    if (raw_points.size() <= 2) {
        log_warning("Polygon %lu too small (region %lu)", polygon_id, region_id);
        return;
    }

    points_converter_t points_converter(allocator_);
    dynarray_t<point_t> points = points_converter.convert(raw_points);

    raw_border_t border;
    border.region_id = region_id;
    border.polygon_id = polygon_id;
    border.square = get_square(points);
    border.type = type;
    border.rectangle = rectangle_t(points.data(), points.size());

    border.edge_refs_offset = geo_data_->raw_edge_refs_number();

    dynarray_t<edge_t> const edges = make_edges(points, geo_data_,
        allocator_, /* change_direction = */ false);

    for (number_t i = 0; i < edges.size(); ++i)
        geo_data_->raw_edge_refs_append(geo_data_->insert(edges[i]));

    border.edge_refs_number = geo_data_->raw_edge_refs_number() - border.edge_refs_offset;
    geo_data_->raw_borders_append(border);
}

void raw_borders_handler_t::update(geo_id_t region_id, proto::polygon_t const &polygon)
{
    locations_converter_t converter(allocator_);
    converter.each(polygon.locations(), [&] (dynarray_t<location_t> const &locations) {
        dynarray_t<point_t> points(locations.size(), allocator_);
        for (location_t const &l : locations)
            points.push_back(point_t(l));
        update(region_id, polygon.polygon_id(), points, (raw_border_t::type_t) polygon.type());
    });
}

void raw_borders_handler_t::update(proto::region_t const &region)
{
    if (!config_.save_raw_borders)
        return;

    for (proto::polygon_t const &polygon : region.polygons())
        update(region.region_id(), polygon);
}

void raw_borders_handler_t::fini()
{
    if (!config_.save_raw_borders)
        return;

    raw_border_t *beg = geo_data_->mut_raw_borders();
    raw_border_t *end = beg + geo_data_->raw_borders_number();

    std::stable_sort(beg, end, [] (raw_border_t const &a, raw_border_t const &b) {
        return a.region_id < b.region_id || (a.region_id == b.region_id && a.square < b.square);
    });
}

} // namespace generator
} // namespace geo_base
