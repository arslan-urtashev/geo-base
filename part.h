#pragma once

#include "algo.h"
#include "edge.h"
#include "point.h"
#include "typedef.h"

namespace troll {

struct part_t {
	coordinate_t coordinate;
	count_t edge_indexes_offset;
	count_t edge_indexes_count;

	bool contains(point_t const &p, index_t const *edge_indexes, edge_t const *edges, point_t const *points) const
	{
		edge_indexes += edge_indexes_offset;
		index_t const *edge_index = lower_bound(
			edge_indexes, edge_indexes_count, p,
			[&] (index_t const &e, point_t const &p)
			{
				if (edges[e].contains(p, points))
					return false;
				point_t const &a = points[edges[e].beg];
				point_t const &b = points[edges[e].end];
				return (b - a).cross(p - a) > 0;
			}
		);
		if (edge_index == edge_indexes + edge_indexes_count)
			return false;
		if (edges[*edge_index].contains(p, points))
			return true;
		return (edge_index - edge_indexes) % 2 == 1;
	}
};

}
