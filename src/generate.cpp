#include "generate.h"

#include "log.h"

namespace troll {

void geo_data_ctx_t::fini(geo_base_alloc_t *base)
{
	geo_data_t *geo_data = base->geo_data();

#define TROLL_DEF_VAR(var_t, var) \
	do { \
		geo_data->var = var; \
	} while (false);

#define TROLL_DEF_PTR(ptr_t, ptr) \
	// undef

#define TROLL_DEF_ARR(arr_t, arr) \
	do { \
		geo_data->arr##_count = arr.size(); \
		geo_data->arr = (arr_t *) base->alloc(sizeof(arr_t) * arr.size()); \
		copy_n(arr.data(), arr.size(), geo_data->arr); \
		arr.clear(); \
		arr.shrink_to_fit(); \
	} while (false);

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

	geo_data->version = geo_data_version();
}

static bool is_bad_edge(edge_t const &e, point_t const *p) {
	return e.beg == e.end || fabs(convert_to_double(p[e.beg].x - p[e.end].x)) > 300.0;
}

static output_t &operator << (output_t &out, point_t const &p)
{
	out << "(" << convert_to_double(p.y) << " " << convert_to_double(p.x) << ")";
	return out;
}

#ifdef TROLL_LOG_BOUNDARY
static output_t &operator << (output_t &out, vector_t<point_t> const &p)
{
	out << "[";
	for (ref_t i = 0; i < p.size(); ++i)
		out << p[i] << (i + 1 == p.size() ? "" : ", ");
	out << "]";
	return out;
}
#endif

static void generate_edges(region_id_t region_id, vector_t<point_t> const &points, geo_data_ctx_t *ctx, vector_t<edge_t> &edges)
{
	edges.clear();
	edges.reserve(points.size());
	for (ref_t i = 0; i < points.size(); ++i) {
		ref_t j = (i + 1 == points.size() ? 0 : i + 1);
		edge_t e(ctx->push_point(points[i]), ctx->push_point(points[j]));
		
		if (ctx->points[e.beg].x > ctx->points[e.end].x)
			swap(e.beg, e.end);

		if (is_bad_edge(e, ctx->points.data())) {
			log_warning("generate", region_id) <<  "bad edge detected = " << ctx->points[e.beg] << " -> " << ctx->points[e.end];
			continue;
		}
		
		edges.push_back(e);
	}
}

static void generate_checkpoints(vector_t<edge_t> const &e, point_t const *p, vector_t<checkpoint_t> &checkpoints)
{
	checkpoints.clear();
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

static square_t square(vector_t<point_t> const &p)
{
	square_t s = 0;
	for (ref_t i = 0; i < p.size(); ++i) {
		ref_t j = (i + 1 == p.size() ? 0 : i + 1);
		s += p[i].cross(p[j]);
	}
	return s > 0 ? s : -s;
}

static uint64_t get_hash(byte_t const *bytes, count_t count, uint64_t base)
{
	uint64_t hash = 0, power = 1;
	for (count_t i = 0; i < count; ++i) {
		hash += (bytes[i] + 1) * power;
		power *= base;
	}
	return hash;
}

static uint64_t get_hash(region_id_t region_id, vector_t<point_t> const &points)
{
	uint64_t hash = 0;
	hash ^= get_hash((byte_t const *) &region_id, sizeof(region_id), 373);
	hash ^= get_hash((byte_t const *) &(points[0]), sizeof(point_t) * points.size(), 337);
	return hash;
}

void generate_t::update(region_id_t region_id, vector_t<point_t> const &points)
{
	if (points.size() <= 2) {
		log_warning("generate", region_id) << "Polygon to small!";
		return;
	}

	uint64_t hash = get_hash(region_id, points);
	if (ctx.processed.find(hash) != ctx.processed.end()) {
		log_error("generate", region_id) << "Region is already processed!";
		return;
	} else {
		ctx.processed.insert(hash);
	}

	polygon_t polygon;
	polygon.region_id = region_id;
	polygon.square = square(points);
	
	polygon.init();
	for (point_t const &p : points)
		polygon.relax(p);

	polygon.parts_offset = ctx.parts.size();
	
	vector_t<edge_t> &edges = ctx.buf.edges;
	generate_edges(region_id, points, &ctx, edges);

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

void generate_t::update(region_id_t region_id, vector_t<location_t> const &raw_locations)
{
	vector_t<location_t> &locations = ctx.buf.locations;
	locations.clear();
	for (location_t const &l : raw_locations)
		if (locations.empty() || locations.back() != l)
			locations.push_back(l);

	log_info("generate", region_id) << "Process locations count = " << locations.size();

	count_t polygons_size = ctx.polygons.size();

	vector_t<point_t> &points = ctx.buf.points;
	points.clear();
	for (ref_t l = 0, r = 0; l < locations.size(); l = r + 1) {
		r = l + 1;
		while (r < locations.size() && locations[l] != locations[r])
			++r;

		if (r + 1 < locations.size() || l > 0)
			log_warning("generate", region_id) << "self-intersections detected " << l << " - " << r;

		points.assign(locations.begin() + l, locations.begin() + r);

#if TROLL_LOG_BOUNDARY
		log_debug("generate", region_id) << points;
#endif

		update(region_id, points);
	}

	log_info("generate", region_id) << ctx.polygons.size() - polygons_size << " polygons generated";
}

void generate_t::create_boxes()
{
	log_info("generate") << "Creating boxes...";

	for (coordinate_t x0 = box_t::LOWER_X; x0 < box_t::UPPER_X; x0 += box_t::DELTA_X) {
		for (coordinate_t y0 = box_t::LOWER_Y; y0 < box_t::UPPER_Y; y0 += box_t::DELTA_Y) {
			box_t box;
			box.polygon_refs_offset = ctx.polygon_refs.size();

			for (ref_t i = 0; i < ctx.polygons.size(); ++i)
				if (ctx.polygons[i].intersect(x0, y0, x0 + box_t::DELTA_X, y0 + box_t::DELTA_Y))
					ctx.polygon_refs.push_back(i);

			box.polygon_refs_count = ctx.polygon_refs.size() - box.polygon_refs_offset;
			ctx.boxes.push_back(box);
		}
	}

	log_info("generate") << "Boxes created";
}

}
