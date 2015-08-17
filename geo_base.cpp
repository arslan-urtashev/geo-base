#include "geo_base.h"

namespace troll {

void geo_base_map(void *dat, geo_base_t *geo_base)
{
#define TROLL_DEF_VAR(var_t, var)
	// undef

#define TROLL_DEF_PTR(ptr_t, ptr) \
	geo_base->ptr =	(ptr_t *) (((mword_t) dat) + ((byte_t *) geo_base->ptr))

#define TROLL_DEF_ARR(arr_t, arr) \
	TROLL_DEF_PTR(arr_t, arr);

	TROLL_DEF_GEO_BASE

#undef TROLL_DEF_PTR
#undef TROLL_DEF_VAR
#undef TROLL_DEF_ARR
}

void geo_base_save(void *dat, geo_base_t *geo_base)
{
#define TROLL_DEF_VAR(var_t, var)
	// undef

#define TROLL_DEF_PTR(ptr_t, ptr) \
	geo_base->ptr = (ptr_t *) (((byte_t *) geo_base->ptr) - ((mword_t) dat));

#define TROLL_DEF_ARR(arr_t, arr) \
	TROLL_DEF_PTR(arr_t, arr);

	TROLL_DEF_GEO_BASE

#undef TROLL_DEF_VAR
#undef TROLL_DEF_PTR
#undef TROLL_DEF_ARR
}

}
