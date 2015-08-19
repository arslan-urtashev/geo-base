#include "generate.h"

namespace troll {

void geo_data_ctx_t::fini(geo_base_alloc_t *base)
{
	geo_data_t *geo_data = base->geo_data();

#define TROLL_DEF_VAR(var_t, var) \
	do { \
		geo_data->var = var; \
	} while (false);

#define TROLL_DEF_ARR(arr_t, arr) \
	do { \
		geo_data->arr##_count = arr.size(); \
		geo_data->arr = (arr_t *) base->alloc(sizeof(arr_t) * arr.size()); \
		copy_n(arr.begin(), arr.size(), geo_data->arr); \
		arr.clear(); \
		arr.shrink_to_fit(); \
	} while (false);

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_ARR
}

static bool is_bad_edge(edge_t const &e, point_t const *p) {
	return e.beg == e.end || fabs(convert_to_double(p[e.beg].x - p[e.end].x)) > 300.0;
}

static void generate_edges(vector_t<point_t> const &points, geo_data_ctx_t *ctx, vector_t<edge_t> &edges)
{
	edges.reserve(points.size());
	for (ref_t i = 0; i < points.size(); ++i) {
		ref_t j = (i + 1 == points.size() ? 0 : i + 1);
		edge_t e(ctx->push_point(points[i]), ctx->push_point(points[j]));
		if (is_bad_edge(e, ctx->points.data()))
			continue;
		if (ctx->points[e.beg].x > ctx->points[e.end].x)
			swap(e.beg, e.end);
		edges.push_back(e);
	}
}

static void generate_checkpoints(vector_t<edge_t> const &e, point_t const *p, vector_t<checkpoint_t> &checkpoints)
{
	checkpoints.reserve(2 * e.size());
	for (ref_t i = 0; i < e.size(); ++i) {
		checkpoints.push_back({p[e[i].beg].x, i, false});
		checkpoints.push_back({p[e[i].end].x, i, true});
	}
	sort(checkpoints.begin(), checkpoints.end(),
		[&] (checkpoint_t const &a, checkpoint_t const &b)
		{
			return a.coordinate < b.coordinate;
		}
	);
}

void generate_t::update(region_id_t region_id, vector_t<point_t> const &points)
{
	polygon_t polygon;
	polygon.region_id = region_id;
	
	polygon.init();
	for (point_t const &p : points)
		polygon.relax(p);

	polygon.parts_offset = ctx.parts.size();
	
	vector_t<edge_t> &edges = ctx.buf.edges;
	generate_edges(points, &ctx, edges);

	vector_t<checkpoint_t> &checkpoints = ctx.buf.checkpoints;
	generate_checkpoints(edges, ctx.points.data(), checkpoints);

	vector_t<edge_t> &erase = ctx.buf.erase;
	for (ref_t l = 0, r = 0; l < checkpoints.size(); l = r) {
		r = l + 1;
		while (r < checkpoints.size() && checkpoints[l].coordinate == checkpoints[r].coordinate)
			++r;

		erase.clear();
		for (ref_t i = l; i < r; ++i)
			if (checkpoints[i].erase)
				erase.push_back(edges[checkpoints[i].cur_edge_ref]);

		sort(erase.begin(), erase.end());

		part_t part;
		part.edge_refs_offset = ctx.edge_refs.size();
		part.coordinate = checkpoints[l].coordinate;

		if (l > 0) {
			part_t const &prev = ctx.parts.back();
			for (ref_t i = prev.edge_refs_offset; i < prev.edge_refs_offset + prev.edge_refs_count; ++i)
				if (!binary_search(erase.begin(), erase.end(), ctx.edges[ctx.edge_refs[i]]))
					ctx.edge_refs.push_back(ctx.edge_refs[i]);
		}

		for (ref_t i = l; i < r; ++i)
			if (!checkpoints[i].erase)
				if (!binary_search(erase.begin(), erase.end(), edges[checkpoints[i].cur_edge_ref]))
					ctx.edge_refs.push_back(ctx.push_edge(edges[checkpoints[i].cur_edge_ref]));

		part.edge_refs_count = ctx.edge_refs.size() - part.edge_refs_offset;

		sort(ctx.edge_refs.begin() + part.edge_refs_offset, ctx.edge_refs.end(),
			[&] (ref_t const &a, ref_t const &b)
			{
				edge_t const &e1 = ctx.edges[a];
				edge_t const &e2 = ctx.edges[b];
				return e1.lower(e2, ctx.points.data());
			}
		);

		ctx.parts.push_back(part);
	}

	polygon.parts_count = ctx.parts.size() - polygon.parts_offset;
	ctx.polygons.push_back(polygon);
}

}
