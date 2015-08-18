#pragma once

#include "algo.h"
#include "edge.h"
#include "part.h"
#include "point.h"
#include "typedef.h"

namespace troll {

struct polygon_t {
	region_id_t region_id;
	polygon_id_t polygon_id;
	coordinate_t upper;
	coordinate_t lower;
	coordinate_t left;
	coordinate_t right;
	offset_t parts_offset;
	count_t parts_count;

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
		part_t const *part = lower_bound(
			parts, parts_count, point,
			[&] (part_t const &a, point_t const &p)
			{
				return a.coordinate <= p.x;
			}
		);
		if (part == parts)
			return false;
		--part;
		if (part + 1 == parts + parts_count)
			--part;
		if (point.x < part->coordinate || point.x > (part + 1)->coordinate)
			return false;
		return part->contains(point, edge_refs, edges, points);
	}
};

}
