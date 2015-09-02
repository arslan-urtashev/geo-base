#pragma once

#include "algo.hpp"
#include "common.hpp"
#include "edge.hpp"
#include "part.hpp"
#include "point.hpp"
#include "region.hpp"

namespace geo_base {

struct polygon_t {
	region_id_t region_id;
	coordinate_t upper;
	coordinate_t lower;
	coordinate_t left;
	coordinate_t right;
	count_t parts_offset;
	count_t parts_count;
	square_t square;
	bool inner;

	void init()
	{
		left = convert_to_coordinate(180.0);
		lower = convert_to_coordinate(90.0);
		right = convert_to_coordinate(-180.0);
		upper = convert_to_coordinate(-90.0);
	}

	void relax(point_t const &p)
	{
		left = std::min(left, p.x);
		lower = std::min(lower, p.y);
		right = std::max(right, p.x);
		upper = std::max(upper, p.y);
	}

	bool intersect(coordinate_t x1, coordinate_t y1, coordinate_t x2, coordinate_t y2) const
	{
		if (x1 > right || x2 < left || y2 < lower || y1 > upper)
			return false;
		return true;
	}

	bool contains(point_t const &point, part_t const *parts, ref_t const *edge_refs, edge_t const *edges, point_t const *points) const
	{
		if (point.x < left || point.x > right || point.y < lower || point.y > upper)
			return false;

		parts += parts_offset;
		part_t const *part = lower_bound(parts, parts_count, point, [&] (part_t const &a, point_t const &p) {
			return a.coordinate <= p.x;
		});

		if (part == parts)
			return false;

		--part;
		if (part + 1 == parts + parts_count)
			--part;

		if (point.x < part->coordinate || point.x > (part + 1)->coordinate)
			return false;

		edge_refs += part->edge_refs_offset;
		count_t edge_refs_count = (part + 1)->edge_refs_offset - part->edge_refs_offset;

		ref_t const *edge_ref =
			lower_bound(edge_refs, edge_refs_count, point, [&] (ref_t const &e, point_t const &p) {
				if (edges[e].contains(p, points))
					return false;
				point_t const &a = points[edges[e].beg];
				point_t const &b = points[edges[e].end];
				return (b - a).cross(p - a) > 0;
			});

		if (edge_ref == edge_refs + edge_refs_count)
			return false;

		if (edges[*edge_ref].contains(point, points))
			return true;

		return (edge_ref - edge_refs) % 2 == 1;
	}

	bool better(polygon_t const &p, region_t const *regions, count_t regions_count) const
	{
		if (square < p.square)
			return true;

		if (square == p.square) {
			region_t const *r1 = find(regions, regions_count, region_id);
			region_t const *r2 = find(regions, regions_count, p.region_id);
			return r1->better(*r2);
		}

		return false;
	}
};

}
