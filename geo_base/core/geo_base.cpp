// Copyright (c) 2015, 2016 Urtashev Arslan. All rights reserved.
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
#include <geo_base/core/geo_data/geo_data.h>

namespace geo_base {

static bool polygon_contains(polygon_t const &p, point_t const &point, geo_data_t const &geo_data)
{
    part_t const *parts = geo_data.parts();
    ref_t const *edge_refs = geo_data.edge_refs();
    edge_t const *edges = geo_data.edges();
    point_t const *points = geo_data.points();
    return p.contains(point, parts, edge_refs, edges, points);
}

static void update_answer(polygon_t const **answer, polygon_t const &polygon,
    geo_data_t const &geo_data)
{
    if (!*answer) {
        *answer = &polygon;
    } else {
        region_t const *regions = geo_data.regions();
        number_t const regions_number = geo_data.regions_number();
        if (!(*answer)->better(polygon, regions, regions_number))
            *answer = &polygon;
    }
}

geo_id_t geo_base_t::lookup(location_t const &location) const
{
    geo_data_t const &geo_data = *geo_data_loader_->geo_data();

    point_t const point(location);

    // Determine in wich area box is point.
    ref_t const box_x = (point.x - area_box::lower_x) / area_box::delta_x;
    ref_t const box_y = (point.y - area_box::lower_y) / area_box::delta_y;
    ref_t const box_ref = box_x * area_box::number_y + box_y;

    if (box_ref >= geo_data.boxes_number())
        return UNKNOWN_GEO_ID;

    number_t const refs_offset = geo_data.boxes()[box_ref].polygon_refs_offset;
    number_t const refs_number = geo_data.boxes()[box_ref].polygon_refs_number;

    polygon_t const *answer = nullptr;

    for (ref_t l = refs_offset, r = 0; l < refs_offset + refs_number; l = r) {
        ref_t const *refs = geo_data.polygon_refs();
        polygon_t const *p = geo_data.polygons();

        r = l + 1;

        if (polygon_contains(p[refs[l]], point, geo_data)) {
            if (p[refs[l]].type == polygon_t::TYPE_INNER) {
                // All polygons with same region_id must be skipped if polygon is inner.
                // In geo_data inner polygons stored before outer polygons.
                while (r < refs_offset + refs_number && p[refs[l]].region_id == p[refs[r]].region_id)
                    ++r;
            } else {
                update_answer(&answer, p[refs[l]], geo_data);
            }
        }
    }

    return answer ? answer->region_id : UNKNOWN_GEO_ID;
}

} // namespace geo_base
