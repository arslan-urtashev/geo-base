#pragma once

#include "common.hpp"
#include "geo_base_alloc.hpp"
#include "geo_data.hpp"
#include "hash.hpp"

#include <unordered_set>
#include <unordered_map>

namespace geo_base {

struct checkpoint_t {
	coordinate_t coordinate;
	ref_t cur_edge_ref;
	bool erase;
};

#define TROLL_DEF_VAR(var_t, var) \
	var_t var;

#define TROLL_DEF_PTR(ptr_t, ptr) \
	// undef

#define TROLL_DEF_ARR(arr_t, arr) \
	std::vector<arr_t> arr;

struct geo_data_ctx_t {
	TROLL_DEF_GEO_DATA

	ref_t push_point(point_t const &p)
	{
		if (saved.points.find(p) == saved.points.end()) {
			saved.points[p] = points.size();
			points.push_back(p);
		}
		return saved.points[p];
	}

	ref_t push_edge(edge_t const &e)
	{
		if (saved.edges.find(e) == saved.edges.end()) {
			saved.edges[e] = edges.size();
			edges.push_back(e);
		}
		return saved.edges[e];
	}

	count_t push_blob(std::string const &b)
	{
		if (saved.blobs.find(b) == saved.blobs.end()) {
			count_t off = blobs.size();
			for (char c : b)
				blobs.push_back(c);
			blobs.push_back('\0');
			saved.blobs[b] = off;
		}
		return saved.blobs[b];
	}

	void fini(geo_base_alloc_t *base);

	double memory() const;

	struct {
		std::vector<checkpoint_t> checkpoints;
		std::vector<edge_t> edges;
		std::vector<edge_t> erase;
		std::vector<point_t> points;
	} buf;

	struct {
		std::unordered_map<edge_t, ref_t, hash64_t> edges;
		std::unordered_map<point_t, ref_t, hash64_t> points;
		std::unordered_map<std::string, count_t> blobs;
	} saved;

	std::unordered_set<uint64_t> processed;
};

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

class geo_base_generate_t {
public:
	geo_base_generate_t(char const *path)
		: base(path)
	{
	}

	void update(region_id_t region_id, std::vector<location_t> const &locations, std::vector<std::string> const &blobs);

	void save();

	void show_base(output_t &out) const
	{
		base.show(out);
	}

	geo_data_ctx_t const &context() const
	{
		return ctx;
	}

private:
	void update(region_id_t region_id, std::vector<point_t> const &points, std::vector<std::string> const &blobs);

	void create_boxes();

	geo_base_alloc_t base;
	geo_data_ctx_t ctx;
	polygon_processor_t processor;
};

}
