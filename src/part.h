#pragma once

#include "algo.h"
#include "edge.h"
#include "point.h"
#include "common.h"

namespace troll {

struct part_t {
	coordinate_t coordinate;
	count_t edge_refs_offset;
	count_t edge_refs_count;

	bool contains(point_t const &p, ref_t const *edge_refs, edge_t const *edges, point_t const *points) const
	{
		edge_refs += edge_refs_offset;
		ref_t const *edge_ref = lower_bound(
			edge_refs, edge_refs_count, p,
			[&] (ref_t const &e, point_t const &p)
			{
				if (edges[e].contains(p, points))
					return false;
				point_t const &a = points[edges[e].beg];
				point_t const &b = points[edges[e].end];
				return (b - a).cross(p - a) > 0;
			}
		);
		if (edge_ref == edge_refs + edge_refs_count)
			return false;
		if (edges[*edge_ref].contains(p, points))
			return true;
		return (edge_ref - edge_refs) % 2 == 1;
	}
};

}
