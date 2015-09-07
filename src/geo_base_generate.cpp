#include "geo_base_generate.hpp"

#include "hash.hpp"
#include "log.hpp"

namespace geo_base {

#ifndef TROLL_GENERATE_LOG_ENABLE
#	define log_debug log_null
#	define log_error log_null
#	define log_info log_null
#	define log_warning log_null
#endif

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
		memcpy(geo_data->arr, arr.data(), sizeof(arr_t) * arr.size()); \
		arr.clear(); \
		arr.shrink_to_fit(); \
	} while (false);

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

	geo_data->version = geo_data_version();
}

double geo_data_ctx_t::memory() const
{
	double total = 0;

#define TROLL_DEF_VAR(var_t, var) \
	total += sizeof(var_t);

#define TROLL_DEF_PTR(ptr_t, ptr) \
	// undef

#define TROLL_DEF_ARR(arr_t, arr) \
	total += sizeof(count_t); \
	total += sizeof(arr_t) * arr.size();

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

	return total * 1.0 / (1024. * 1024.);
}

static bool is_bad_edge(edge_t const &e, point_t const *p) {
	return e.beg == e.end || fabs(convert_to_double(p[e.beg].x - p[e.end].x)) > 300.0;
}

static void generate_edges(region_id_t region_id, std::vector<point_t> const &points, geo_data_ctx_t *ctx, std::vector<edge_t> &edges)
{
	edges.clear();
	edges.reserve(points.size());
	for (ref_t i = 0; i < points.size(); ++i) {
		ref_t j = (i + 1 == points.size() ? 0 : i + 1);
		edge_t e(ctx->push_point(points[i]), ctx->push_point(points[j]));
		
		if (ctx->points[e.beg].x > ctx->points[e.end].x)
			std::swap(e.beg, e.end);

		if (is_bad_edge(e, ctx->points.data())) {
			log_warning("generate", region_id) <<  "bad edge detected = " << ctx->points[e.beg] << " -> " << ctx->points[e.end];
			continue;
		}
		
		edges.push_back(e);
	}
}

static void generate_checkpoints(std::vector<edge_t> const &e, point_t const *p, std::vector<checkpoint_t> &checkpoints)
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

static square_t square(std::vector<point_t> const &p)
{
	square_t s = 0;
	for (ref_t i = 0; i < p.size(); ++i) {
		ref_t j = (i + 1 == p.size() ? 0 : i + 1);
		s += p[i].cross(p[j]);
	}
	return s > 0 ? s : -s;
}

static uint64_t get_hash(region_id_t region_id, std::vector<point_t> const &points)
{
	uint64_t hash = 0;
	hash ^= poly_hash_t<373>()(region_id);
	hash ^= poly_hash_t<337>()(points);
	return hash;
}

void geo_base_generate_t::update(region_id_t region_id, polygon_id_t polygon_id, std::vector<point_t> const &points, bool inner)
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
	polygon.polygon_id = polygon_id;
	polygon.square = square(points);
	polygon.inner = inner;
	
	polygon.init();
	for (point_t const &p : points)
		polygon.relax(p);

	polygon.parts_offset = ctx.parts.size();
	
	std::vector<edge_t> &edges = ctx.buf.edges;
	generate_edges(region_id, points, &ctx, edges);

	std::vector<checkpoint_t> &checkpoints = ctx.buf.checkpoints;
	generate_checkpoints(edges, ctx.points.data(), checkpoints);

	std::vector<edge_t> &erase = ctx.buf.erase;
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
			for (ref_t i = prev.edge_refs_offset; i < part.edge_refs_offset; ++i)
				if (!binary_search(erase.begin(), erase.end(), ctx.edges[ctx.edge_refs[i]]))
					ctx.edge_refs.push_back(ctx.edge_refs[i]);
		}

		for (ref_t i = l; i < r; ++i)
			if (!checkpoints[i].erase)
				if (!binary_search(erase.begin(), erase.end(), edges[checkpoints[i].cur_edge_ref]))
					ctx.edge_refs.push_back(ctx.push_edge(edges[checkpoints[i].cur_edge_ref]));

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

void geo_base_generate_t::update(region_id_t region_id, proto::polygon_t const &polygon)
{
	std::vector<point_t> &points = ctx.buf.points;

	std::vector<location_t> &locations = ctx.buf.locations;
	locations.assign(polygon.locations().begin(), polygon.locations().end());

	processor(locations, [&] (std::vector<location_t> const &locations) {
		points.assign(locations.begin(), locations.end());
		update(region_id, polygon.polygon_id(), points, polygon.type() == proto::polygon_t::TYPE_INNER);
	});
}

void geo_base_generate_t::update(proto::geo_data_t const &data)
{
	for (proto::polygon_t const &p : data.polygons())
		update(data.region_id(), p);

	region_t region;
	region.region_id = data.region_id();
	region.kvs_offset = ctx.kvs.size();

	for (proto::kv_t const &x : data.kvs()) {
		kv_t kv;
		kv.k = ctx.push_blob(x.k());
		kv.v = ctx.push_blob(x.v());
		ctx.kvs.push_back(kv);
	}

	region.kvs_count = ctx.kvs.size() - region.kvs_offset;
	ctx.regions.push_back(region);
}

#ifndef TROLL_GENERATE_LOG_ENABLE
#	undef log_debug
#	undef log_error
#	undef log_info
#	undef log_warning
#endif

void geo_base_generate_t::create_boxes()
{
	log_info("generate") << "Generate boxes...";

	for (coordinate_t x0 = box_t::LOWER_X; x0 < box_t::UPPER_X; x0 += box_t::DELTA_X) {
		for (coordinate_t y0 = box_t::LOWER_Y; y0 < box_t::UPPER_Y; y0 += box_t::DELTA_Y) {
			box_t box;
			box.polygon_refs_offset = ctx.polygon_refs.size();

			for (ref_t i = 0; i < ctx.polygons.size(); ++i)
				if (ctx.polygons[i].intersect(x0, y0, x0 + box_t::DELTA_X, y0 + box_t::DELTA_Y))
					ctx.polygon_refs.push_back(i);

			box.polygon_refs_count = ctx.polygon_refs.size() - box.polygon_refs_offset;

			std::sort(ctx.polygon_refs.begin() + box.polygon_refs_offset, ctx.polygon_refs.end(),
				[&] (ref_t const &a, ref_t const &b) {
					polygon_t const *p = ctx.polygons.data();
					return p[a].region_id < p[b].region_id || (p[a].region_id == p[b].region_id && p[a].inner && !p[b].inner);
				}
			);

			ctx.boxes.push_back(box);
		}
	}

	log_info("generate") << "Boxes generated";
}

void geo_base_generate_t::save()
{
	create_boxes();

	sort(ctx.regions.begin(), ctx.regions.end());
	for (region_t &r : ctx.regions)
		r.square = 0;

	for (polygon_t const &p : ctx.polygons) {
		region_t *region = find(ctx.regions.data(), ctx.regions.size(), p.region_id);
		if (region)
			region->square += p.square;
	}

	part_t fake_part;
	fake_part.coordinate = 0;
	fake_part.edge_refs_offset = ctx.edge_refs.size();
	ctx.parts.push_back(fake_part);
	
	ctx.fini(&base);
}

}
