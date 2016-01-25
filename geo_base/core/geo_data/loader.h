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

#include <memory>
#include <geo_base/core/geo_data/geo_data.h>
#include <geo_base/core/geo_data/map.h>
#include <geo_base/lib/mem_file.h>

namespace geo_base {

class geo_data_loader_t {
public:
    virtual geo_data_t const *geo_data() const = 0;

    virtual ~geo_data_loader_t()
    { }
};

using geo_data_loader_ptr_t = std::unique_ptr<geo_data_loader_t>;

class geo_data_map_loader_t : public geo_data_loader_t {
public:
    explicit geo_data_map_loader_t(char const *path)
    {
        mem_file_.read_open(path);
        geo_data_ = geo_data_map_t((char const *) mem_file_.data(), mem_file_.size());
    }

    geo_data_t const *geo_data() const override
    {
        return &geo_data_;
    }

private:
    mem_file_t mem_file_;
    geo_data_map_t geo_data_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(geo_data_map_loader_t);
};

class geo_data_wrapper_t : public geo_data_loader_t {
public:
    explicit geo_data_wrapper_t(geo_data_t const &g)
        : geo_data_(&g)
    { }

    geo_data_t const *geo_data() const override
    {
        return geo_data_;
    }

private:
    geo_data_t const *geo_data_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(geo_data_wrapper_t);
};

} // namespace geo_base
