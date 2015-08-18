#include "geo_base_alloc.h"
#include "vector.h"

#include <iostream>

using namespace troll;

#define TROLL_DEF_VAR(var_t, var) \
	var_t var;

#define TROLL_DEF_PTR(ptr_t, ptr) \
	// undef

#define TROLL_DEF_ARR(arr_t, arr) \
	vector_t<arr_t> arr;

struct context_t {
	TROLL_DEF_GEO_DATA
};

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR

static void context_save(geo_base_alloc_t *alloc, context_t *context)
{
	geo_data_t *geo_data = alloc->geo_data();

#define TROLL_DEF_VAR(var_t, var) \
	do { \
		geo_data->var = context->var; \
	} while (false);

#define TROLL_DEF_PTR(ptr_t, ptr) \
	// undef

#define TROLL_DEF_ARR(arr_t, arr) \
	do { \
		geo_data->arr##_count = context->arr.size(); \
		geo_data->arr = (arr_t *) alloc->alloc(sizeof(arr_t) * context->arr.size()); \
		copy_n(context->arr.begin(), context->arr.size(), geo_data->arr); \
		context->arr.clear(); \
		context->arr.shrink_to_fit(); \
	} while (false);

	TROLL_DEF_GEO_DATA

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR
}

static void usage()
{
	std::cerr << "geo-base-generate <geo.dat>" << std::endl;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	if (argc != 2) {
		usage();
		return -1;
	}

	geo_base_alloc_t base(argv[1]);

	context_t context;

	context_save(&base, &context);

	return 0;
}
