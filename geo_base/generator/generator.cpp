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

#include <geo_base/core/geo_data/map.h>
#include <geo_base/generator/generator.h>
#include <geo_base/generator/geo_data.h>
#include <geo_base/generator/mut_geo_data.h>
#include <geo_base/generator/locations_converter.h>
#include <geo_base/lib/base_allocator.h>
#include <geo_base/lib/pool_allocator.h>
#include <geo_base/lib/log.h>
#include <geo_base/lib/stop_watch.h>
#include <geo_base/libproto/proto_reader.h>

namespace geo_base {
namespace generator {

struct check_point_t {
    coordinate_t x;
    number_t idx;
    bool is_start;
};

static bool is_bad_edge(edge_t const &e, point_t const *p)
{
    return abs(p[e.beg].x - p[e.end].x) > to_coordinate(300.0);
}

static dynarray_t<edge_t> make_edges(dynarray_t<point_t> const &points, geo_data_t *geo_data,
    allocator_t *allocator)
{
    dynarray_t<edge_t> edges(points.size(), allocator);
    for (number_t i = 0; i < points.size(); ++i) {
        number_t j = (i + 1 == points.size() ? 0 : i + 1);

        if (points[i] == points[j])
            continue;

        ref_t const &p1 = geo_data->insert(points[i]);
        ref_t const &p2 = geo_data->insert(points[j]);
        edge_t e(p1, p2);

        point_t const *p = geo_data->points();
        if (p[e.beg].x > p[e.end].x)
            std::swap(e.beg, e.end);

        if (is_bad_edge(e, p)) {
            log_warning("Bad edge (%f, %f)=>(%f, %f)",
                to_double(p[e.beg].x), to_double(p[e.beg].y),
                to_double(p[e.end].x), to_double(p[e.end].y));
            continue;
        }

        edges.push_back(e);
    }

    return edges;
}

static dynarray_t<check_point_t> make_check_points(dynarray_t<edge_t> const &e,
    geo_data_t *geo_data, allocator_t *allocator)
{
    dynarray_t<check_point_t> check_points(e.size() * 2, allocator);
    point_t const *p = geo_data->points();

    for (number_t i = 0; i < e.size(); ++i) {
        check_points.push_back(check_point_t{p[e[i].beg].x, i, true});
        check_points.push_back(check_point_t{p[e[i].end].x, i, false});
    }

    std::stable_sort(check_points.begin(), check_points.end(),
        [&] (check_point_t const &a, check_point_t const &b) {
            return a.x < b.x;
        }
    );

    return check_points;
}

static square_t get_square(dynarray_t<point_t> const &p)
{
    square_t s = 0;
    for (number_t i = 0; i < p.size(); ++i) {
        number_t j = (i + 1 == p.size() ? 0 : i + 1);
        s += p[i].cross(p[j]);
    }
    return s > 0 ? s : -s;
}

void generator_t::update(geo_id_t region_id, geo_id_t polygon_id, dynarray_t<point_t> const &points,
    polygon_t::type_t type)
{
    if (points.size() <= 2) {
        log_warning("Polygon %lu too small (region %lu)", polygon_id, region_id);
        return;
    }

    polygon_t polygon;
    memset(&polygon, 0, sizeof(polygon));

    polygon.region_id = region_id;
    polygon.polygon_id = polygon_id;
    polygon.square = get_square(points);
    polygon.type = type;
    polygon.rectangle = rectangle_t(points.data(), points.size());

    polygon.parts_offset = geo_data_->parts_number();

    dynarray_t<edge_t> const edges = make_edges(points, geo_data_, allocator_);
    dynarray_t<check_point_t> const check_points = make_check_points(edges, geo_data_, allocator_);

    for (number_t l = 0, r = 0; l < check_points.size(); l = r) {
        r = l + 1;
        while (r < check_points.size() && check_points[l].x == check_points[r].x)
            ++r;

        dynarray_t<edge_t> erase(r - l, allocator_);
        for (number_t i = l; i < r; ++i)
            if (!check_points[i].is_start)
                erase.push_back(edges[check_points[i].idx]);

        std::stable_sort(erase.begin(), erase.end());

        part_t part;
        part.edge_refs_offset = geo_data_->edge_refs_number();
        part.coordinate = check_points[l].x;

        if (l > 0) {
            part_t const &prev = *(geo_data_->parts() + geo_data_->parts_number() - 1);
            for (number_t i = prev.edge_refs_offset; i < part.edge_refs_offset; ++i) {
                edge_t const &e = geo_data_->edges()[geo_data_->edge_refs()[i]];
                if (!std::binary_search(erase.begin(), erase.end(), e))
                    geo_data_->edge_refs_append(geo_data_->edge_refs()[i]);
            }
        }

        for (number_t i = l; i < r; ++i)
            if (check_points[i].is_start)
                if (!std::binary_search(erase.begin(), erase.end(), edges[check_points[i].idx]))
                    geo_data_->edge_refs_append(geo_data_->insert(edges[check_points[i].idx]));

        ref_t *mut_edge_refs = geo_data_->mut_edge_refs() + part.edge_refs_offset;
        ref_t *mut_edge_refs_end = geo_data_->mut_edge_refs() + geo_data_->edge_refs_number();

        std::stable_sort(mut_edge_refs, mut_edge_refs_end, [&] (ref_t const &a, ref_t const &b) {
            edge_t const &e1 = geo_data_->edges()[a];
            edge_t const &e2 = geo_data_->edges()[b];
            return e1.lower(e2, geo_data_->points());
        });

        geo_data_->parts_append(part);
    }

    polygon.parts_number = geo_data_->parts_number() - polygon.parts_offset;
    geo_data_->polygons_append(polygon);
}

void generator_t::init()
{
    log_info("Generate polygons...");

    stop_watch_.run();
}

static void sort_polygons(geo_data_t *g)
{
    polygon_t *begin = g->mut_polygons();
    polygon_t *end = begin + g->polygons_number();

    std::stable_sort(begin, end, [] (polygon_t const &a, polygon_t const &b) {
        return a.rectangle.x2 < b.rectangle.x2;
    });
}

static number_t find_offset(geo_data_t const *g, coordinate_t x)
{
    polygon_t const *begin = g->polygons();
    polygon_t const *end = begin + g->polygons_number();
    return std::lower_bound(begin, end, x, [&] (polygon_t const &p, coordinate_t x) {
        return p.rectangle.x2 < x;
    }) - begin;
}

void generator_t::generate_area_boxes()
{
    log_info("Generate area boxes...");

    stop_watch_t stop_watch;
    stop_watch.run();

    sort_polygons(geo_data_);

    for (coordinate_t x0 = area_box::lower_x; x0 < area_box::upper_x; x0 += area_box::delta_x) {
        number_t const offset = find_offset(geo_data_, x0);

        for (coordinate_t y0 = area_box::lower_y; y0 < area_box::upper_y; y0 += area_box::delta_y) {
            rectangle_t rect(x0, y0, x0 + area_box::delta_x, y0 + area_box::delta_y);

            area_box_t box;
            box.polygon_refs_offset = geo_data_->polygon_refs_number();

            number_t const polygons_number = geo_data_->polygons_number();
            polygon_t const *polygons = geo_data_->polygons();

            for (number_t i = offset; i < polygons_number; ++i)
                if (polygons[i].rectangle.has_intersection(rect))
                    geo_data_->polygon_refs_append(i);

            box.polygon_refs_number = geo_data_->polygon_refs_number() - box.polygon_refs_offset;

            ref_t *mut_polygon_refs = geo_data_->mut_polygon_refs() + box.polygon_refs_offset;
            ref_t *mut_polygon_refs_end = geo_data_->mut_polygon_refs() + geo_data_->polygon_refs_number();

            std::stable_sort(mut_polygon_refs, mut_polygon_refs_end, [&] (ref_t const &a, ref_t const &b) {
                return polygons[a].region_id < polygons[b].region_id
                    || (
                        polygons[a].region_id == polygons[b].region_id
                        && polygons[a].type < polygons[b].type
                    )
                    || (
                        polygons[a].region_id == polygons[b].region_id
                        && polygons[a].type == polygons[b].type
                        && memcmp(&polygons[a], &polygons[b], sizeof(polygon_t)) < 0   
                    );
            });

            geo_data_->boxes_append(box);
        }
    }

    log_info("Area boxes generated in %.3f seconds", stop_watch.get());
}

// Create fake part for right edge_refs_number calculation.
static void create_fake_part(geo_data_t *geo_data)
{
    part_t fake_part;
    fake_part.coordinate = 0;
    fake_part.edge_refs_offset = geo_data->edge_refs_number();
    geo_data->parts_append(fake_part);
}

void generator_t::final_update_regions()
{
    region_t *regions = geo_data_->mut_regions();
    region_t *regions_end = geo_data_->regions_number() + regions;

    std::stable_sort(regions, regions_end);

    for (size_t i = 0; i < geo_data_->polygons_number(); ++i) {
        polygon_t const &p = geo_data_->polygons()[i];
        region_t *r = std::lower_bound(regions, regions_end, p.region_id);
        if (r == regions_end || r->region_id != p.region_id) {
            log_warning("Region %lu not exists!", p.region_id);
            continue;
        }
        r->square += p.square;
        r->polygons_number++;
    }
}

void generator_t::fini()
{
    log_info("Polygons generated in %.3f seconds", stop_watch_.get());

    generate_area_boxes();
    final_update_regions();
    create_fake_part(geo_data_);

    geo_data_->set_version(GEO_DATA_CURRENT_VERSION);
}

void generator_t::update(geo_id_t region_id, proto::polygon_t const &polygon)
{
    locations_converter_t converter(allocator_);
    converter.each(polygon.locations(), [&] (dynarray_t<location_t> const &locations) {
        dynarray_t<point_t> points(locations.size(), allocator_);
        for (location_t const &l : locations)
            points.push_back(point_t(l));
        update(region_id, polygon.polygon_id(), points, (polygon_t::type_t) polygon.type());
    });
}

void generator_t::update(proto::region_t const &proto_region)
{
    for (proto::polygon_t const &polygon : proto_region.polygons())
        update(proto_region.region_id(), polygon);

    region_t region;
    memset(&region, 0, sizeof(region));

    region.region_id = proto_region.region_id();
    region.kvs_offset = geo_data_->kvs_number();

    for (proto::kv_t const &kv : proto_region.kvs()) {
        kv_t x;
        x.k = geo_data_->insert(kv.k());
        x.v = geo_data_->insert(kv.v());
        geo_data_->kvs_append(x);
    }

    region.kvs_number = geo_data_->kvs_number() - region.kvs_offset;
    geo_data_->regions_append(region);
}

void generate(char const *in, char const *out)
{
    pool_allocator_t allocator(128_mb);

    mut_geo_data_t geo_data;
    generator_t generator(&geo_data, &allocator);

    generator.init();

    proto_reader_t reader(in);
    reader.each([&] (proto::region_t const &region) {
        generator.update(region);
    });

    generator.fini();

    base_allocator_t base_allocator(out);
    geo_data_map_t map(geo_data, &base_allocator);
}

} // namespace generator
} // namespace geo_base
