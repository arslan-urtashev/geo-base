// Copyright (c) 2016 Urtashev Arslan. All rights reserved.
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

#include <geo_base/core/point.h>
#include <geo_base/library/allocator.h>
#include <geo_base/library/dynarray.h>

namespace geo_base {
namespace generator {

class points_converter_t {
public:
    explicit points_converter_t(allocator_t *allocator)
        : allocator_(allocator)
    { }

    static bool update(point_t *a, point_t *b)
    {
        if (a->x == b->x) {
            if (a->y < b->y) {
                ++a->x;
            } else {
                ++b->x;
            }
            return true;
        } else {
            return false;
        }
    }

    template<typename points_t>
    dynarray_t<point_t> convert(points_t const &points)
    {
        if (points.empty())
            return dynarray_t<point_t>();

        dynarray_t<point_t> ret(points.size() + 1, points.size() + 1, allocator_);

        for (number_t i = 0; i < points.size(); ++i)
            ret[i] = points[i];
        ret[points.size()] = points[0];

        number_t i = 1;
        while (i < ret.size()) {
            if (update(&ret[i], &ret[i - 1])) {
                i = 1;
            } else {
                ++i;
            }
        }

        if (ret.back() == ret.front())
            ret.pop_back();

        return ret;
    }

private:
    allocator_t *allocator_;
};

} // namespace generator
} // namespace geo_base
