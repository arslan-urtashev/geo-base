// Copyright (c) 2015, 2016 Urtashev Arslan. All rights reserved.
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

#include <geo_base/core/geo_data.h>

namespace geo_base {

geo_id_t geo_data_t::lookup(location_t const &location)
{
	point_t point(location);

	// Determine in wich area box is point.
	ref_t box_x = (point.x - area_box::lower_x) / area_box::delta_x;
	ref_t box_y = (point.y - area_box::lower_y) / area_box::delta_y;
	ref_t box_ref = box_x * area_box::number_y + box_y;

	if (box_ref >= boxes_number())
		return UNKNOWN_GEO_ID;

	number_t refs_offset = boxes()[box_ref].polygon_refs_offset;
	number_t refs_number = boxes()[box_ref].polygon_refs_number;

	polygon_t const *answer = nullptr;

	for (ref_t l = refs_offset, r = 0; l < refs_offset + refs_number; l = r) {
		ref_t const *refs = polygon_refs();
		polygon_t const *p = polygons();

		r = l + 1;

		if (p[refs[l]].contains(point, parts(), edge_refs(), edges(), points())) {
			if (p[refs[l]].type == polygon_t::TYPE_INNER) {
				// All polygons with same region_id must be skipped if polygon is inner.
				// In geo_data inner polygons stored before outer polygons.
				while (r < refs_offset + refs_number && p[refs[l]].region_id == p[refs[r]].region_id)
					++r;

			} else {
				if (!answer || !answer->better(p[refs[l]], regions(), regions_number()))
					answer = &(p[refs[l]]);
			}
		}
	}

	return answer ? answer->region_id : UNKNOWN_GEO_ID;
}

} // namespace geo_base
