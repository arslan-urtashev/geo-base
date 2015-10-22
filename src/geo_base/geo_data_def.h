// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include "area_box.h"
#include "edge.h"
#include "kv.h"
#include "part.h"
#include "point.h"
#include "polygon.h"
#include "region.h"
#include "typedef.h"

namespace geo_base {

// Geographical data definition. This define need for reflection in map/unmap, show, etc.
#define GEO_BASE_DEF_GEO_DATA \
	/* Version of geographical data. Need for check that library can parse this geo-data. */ \
	GEO_BASE_DEF_VAR(version_t, version); \
	/* Array of points. Other structures contains only references on elements in this array. */ \
	GEO_BASE_DEF_ARR(point_t, points); \
	/* Array of edges. Other structures contains only references on elements in this array. */ \
	GEO_BASE_DEF_ARR(edge_t, edges); \
	/* Array with persistence scanline versions data. */ \
	GEO_BASE_DEF_ARR(ref_t, edge_refs); \
	/* Array with persistence scanline version definitions. */ \
	GEO_BASE_DEF_ARR(part_t, parts); \
	/* Geographical polygons. */ \
	GEO_BASE_DEF_ARR(polygon_t, polygons); \
	/* References on polygons. See area_box.h for details. */ \
	GEO_BASE_DEF_ARR(ref_t, polygon_refs); \
	/* Area boxes. See area_box.h for details. */ \
	GEO_BASE_DEF_ARR(area_box_t, boxes); \
	/* All blobs lie in here. */ \
	GEO_BASE_DEF_ARR(byte_t, blobs); \
	/* References on blobs. Stored in sorted order by key for every region. */ \
	GEO_BASE_DEF_ARR(kv_t, kvs); \
	/* Geographical regions. */ \
	GEO_BASE_DEF_ARR(region_t, regions);

// Check that all geo_data types is tribially copyable. Need for correct placement in geo_data
// file.
#define GEO_BASE_DEF_VAR(var_t, var) \
	static_assert(std::is_trivially_copyable<var_t>::value, #var " must be trivially copyable!");
#define GEO_BASE_DEF_ARR(arr_t, arr) \
	static_assert(std::is_trivially_copyable<arr_t>::value, #arr " must be trivially copyable!");

GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

} // namespace geo_base
