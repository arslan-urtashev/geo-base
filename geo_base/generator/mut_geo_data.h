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

#pragma once

#include <geo_base/generator/geo_data.h>
#include <geo_base/library/dynarray.h>
#include <geo_base/library/pool_allocator.h>

#include <vector>
#include <string>
#include <unordered_map>

namespace geo_base {
namespace generator {

class mut_geo_data_t : public geo_data_t {
#define GEO_BASE_DEF_VAR(var_t, var) \
public: \
    var_t const &var() const override \
    { \
        return var##_; \
    } \
    void set_##var(var_t const &var) override \
    { \
        var##_ = var; \
    } \
private: \
    var_t var##_;

#define GEO_BASE_DEF_ARR(arr_t, arr) \
public: \
    arr_t const *arr() const override \
    { \
        return arr##_.data(); \
    } \
    arr_t *mut_##arr() override \
    { \
        return arr##_.data(); \
    } \
    number_t arr##_number() const override \
    { \
        return arr##_.size(); \
    } \
    void arr##_append(arr_t const &a) override \
    { \
        arr##_.push_back(a); \
    } \
private: \
    std::vector<arr_t> arr##_;

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

public:
    mut_geo_data_t()
    { }

    ref_t insert(point_t const &p) override
    {
        if (point_ref_.find(p) == point_ref_.end()) {
            point_ref_[p] = points_.size();
            points_.push_back(p);
        }
        return point_ref_[p];
    }

    ref_t insert(edge_t const &e) override
    {
        if (edge_ref_.find(e) == edge_ref_.end()) {
            edge_ref_[e] = edges_.size();
            edges_.push_back(e);
        }
        return edge_ref_[e];
    }

    ref_t insert(std::string const &s) override
    {
        if (blob_ref_.find(s) == blob_ref_.end()) {
            blob_ref_[s] = blobs_.size();
            for (size_t i = 0; i < s.size(); ++i)
                blobs_.push_back(s[i]);
            blobs_.push_back('\0');
        }
        return blob_ref_[s];
    }

private:
    template<typename type_t>
    struct hash64_t {
        std::hash<uint64_t> hash;

        uint64_t operator () (type_t const &x) const
        {
            static_assert(sizeof(x) == sizeof(uint64_t), "Data sizes must be equal to 8 bytes");
            return hash(*((uint64_t const *) &x));
        }
    };

    std::unordered_map<edge_t, ref_t, hash64_t<edge_t>> edge_ref_;
    std::unordered_map<point_t, ref_t, hash64_t<point_t>> point_ref_;
    std::unordered_map<std::string, ref_t> blob_ref_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(mut_geo_data_t);
};

} // namespace generator
} // namespace geo_base
