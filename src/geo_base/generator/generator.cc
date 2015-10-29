// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
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

#include <geo_base/generator/generator.h>
#include <geo_base/generator/geo_data.h>
#include <geo_base/generator/locations_converter.h>
#include <geo_base/util/log.h>

namespace geo_base {
namespace generator {

struct check_point_t {
	coordinate_t x;
	count_t idx;
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
	for (count_t i = 0; i < points.size(); ++i) {
		count_t j = (i + 1 == points.size() ? 0 : i + 1);

		if (points[i] == points[j])
			continue;

		edge_t e(geo_data->insert(points[i]), geo_data->insert(points[j]));

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

	for (count_t i = 0; i < e.size(); ++i) {
		check_points.push_back(check_point_t{p[e[i].beg].x, i, true});
		check_points.push_back(check_point_t{p[e[i].end].x, i, false});
	}

	std::sort(check_points.begin(), check_points.end(),
		[&] (check_point_t const &a, check_point_t const &b) {
			return a.x < b.x;
		}
	);

	return check_points;
}

static square_t get_square(dynarray_t<point_t> const &p)
{
	square_t s = 0;
	for (count_t i = 0; i < p.size(); ++i) {
		count_t j = (i + 1 == p.size() ? 0 : i + 1);
		s += p[i].cross(p[j]);
	}
	return s > 0 ? s : -s;
}

void generator_t::update(geo_id_t region_id, geo_id_t polygon_id,
	dynarray_t<point_t> const &points, bool is_inner)
{
	if (points.size() <= 2) {
		log_warning("Polygon %lu too small (region %lu)", polygon_id, region_id);
		return;
	}

	polygon_t polygon;
	polygon.region_id = region_id;
	polygon.polygon_id = polygon_id;
	polygon.square = get_square(points);
	polygon.is_inner = is_inner;
	polygon.rectangle = rectangle_t(points.data(), points.size());

	polygon.parts_offset = geo_data_->parts_count();

	dynarray_t<edge_t> edges = make_edges(points, geo_data_, allocator_);
	dynarray_t<check_point_t> check_points = make_check_points(edges, geo_data_, allocator_);

	for (count_t l = 0, r = 0; l < check_points.size(); l = r) {
		r = l + 1;
		while (r < check_points.size() && check_points[l].x == check_points[r].x)
			++r;

		dynarray_t<edge_t> erase(r - l, allocator_);
		for (count_t i = l; i < r; ++i)
			if (!check_points[i].is_start)
				erase.push_back(edges[check_points[i].idx]);

		std::sort(erase.begin(), erase.end());

		part_t part;
		part.edge_refs_offset = geo_data_->edge_refs_count();
		part.coordinate = check_points[l].x;

		if (l > 0) {
			part_t const &prev = *(geo_data_->parts() + geo_data_->parts_count() - 1);
			for (count_t i = prev.edge_refs_offset; i < part.edge_refs_offset; ++i) {
				edge_t const &e = geo_data_->edges()[geo_data_->edge_refs()[i]];
				if (!std::binary_search(erase.begin(), erase.end(), e))
					geo_data_->edge_refs_append(geo_data_->edge_refs()[i]);
			}
		}

		for (count_t i = l; i < r; ++i)
			if (check_points[i].is_start)
				if (!std::binary_search(erase.begin(), erase.end(), edges[check_points[i].idx]))
					geo_data_->edge_refs_append(geo_data_->insert(edges[check_points[i].idx]));

		std::sort(
			geo_data_->mut_edge_refs() + part.edge_refs_offset,
			geo_data_->mut_edge_refs() + geo_data_->edge_refs_count(),
			[&] (ref_t const &a, ref_t const &b) {
				edge_t const &e1 = geo_data_->edges()[a];
				edge_t const &e2 = geo_data_->edges()[b];
				return e1.lower(e2, geo_data_->points());
			}
		);

		geo_data_->parts_append(part);
	}

	polygon.parts_count = geo_data_->parts_count() - polygon.parts_offset;
	geo_data_->polygons_append(polygon);
}

static bool is_inner(proto::polygon_t const &p)
{
	return p.type() == proto::polygon_t::TYPE_INNER;
}

void generator_t::update(geo_id_t region_id, proto::polygon_t const &polygon)
{
	locations_converter_t converter(allocator_);
	converter.each(polygon.locations(), [&] (dynarray_t<location_t> const &locations) {
		dynarray_t<point_t> points(locations.size(), allocator_);
		for (location_t const &l : locations)
			points.push_back(point_t(l));
		update(region_id, polygon.polygon_id(), points, is_inner(polygon));
	});
}

void generator_t::update(proto::region_t const &region)
{
	for (proto::polygon_t const &polygon : region.polygons())
		update(region.region_id(), polygon);
}

} // namespace generator
} // namespace geo_base
