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

#include <geo_base/core/geo_data/map.h>
#include <geo_base/library/log.h>
#include <geo_base/library/system.h>
#include <geo_base/library/crc32.h>
#include <geo_base/proto/geo_data.pb.h>

#include <cstdint>

namespace geo_base {

static size_t const CRC_SIZE = 512;

void geo_data_map_t::init()
{
#define GEO_BASE_DEF_VAR(var_t, var) \
    var##_ = var_t();

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    arr##_ = nullptr; \
    arr##_number_ = 0;

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR
}

geo_data_map_t::geo_data_map_t()
    : data_(nullptr)
    , size_(0)
{
    init();
}

static bool check_memory_consistency(proto::geo_data_t const &g)
{
    std::vector<std::pair<intptr_t, intptr_t>> segments;

#define GEO_BASE_DEF_VAR(var_t, var) \
    // undef

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    if (g.arr##_number() > 0) { \
        intptr_t const beg = g.arr(); \
        intptr_t const end = g.arr() + g.arr##_number() * sizeof(arr_t); \
        segments.emplace_back(beg, end); \
    }

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    std::sort(segments.begin(), segments.end());

    for (size_t i = 0; i + 1 < segments.size(); ++i)
        if (segments[i].second > segments[i + 1].first)
            return false;

    return true;
}

void geo_data_map_t::remap()
{
    init();

    if (!data_)
        return;

    uint64_t const header_size = ntohl(*((uint64_t const *) data_));

    proto::geo_data_t header;
    if (!header.ParseFromArray(data_ + sizeof(uint64_t), header_size))
        throw exception_t("Unable parse geo_data header");

    if (header.magic() != SYSTEM_ENDIAN_FLAG)
        throw exception_t("Different endianness in geo_data and host");
    
    if (!check_memory_consistency(header))
        throw exception_t("Memory is not consistent!");

#define GEO_BASE_DEF_VAR(var_t, var) \
    var##_ = header.var();

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    GEO_BASE_DEF_VAR(number_t, arr##_number); \
    if (arr##_number() > 0) { \
        intptr_t const offset = header.arr(); \
        arr##_ = (arr_t *) (((intptr_t) data_) + offset); \
        uint32_t const hash = crc32(arr##_, std::min(arr##_number_ * sizeof(arr_t), CRC_SIZE)); \
        if (hash != header.arr##_crc32()) \
            throw exception_t("Wrong crc32 for %s", #arr); \
    } while (false);

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    if (version() != GEO_DATA_CURRENT_VERSION)
        throw exception_t("Unable use version %llu (current version is %llu)",
            version(), GEO_DATA_CURRENT_VERSION);
}

static size_t header_size()
{
    proto::geo_data_t header;
    header.set_magic(std::numeric_limits<decltype(header.magic())>::max());

#define GEO_BASE_DEF_VAR(var_t, var) \
    header.set_##var(std::numeric_limits<decltype(header.var())>::max());

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    GEO_BASE_DEF_VAR(number_t, arr##_number); \
    header.set_##arr(std::numeric_limits<decltype(header.arr())>::max()); \
    header.set_##arr##_crc32(std::numeric_limits<decltype(header.arr##_crc32())>::max());

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR
    
    return header.ByteSize();
}

static char const *serialize(geo_data_t const &g, block_allocator_t *allocator, size_t *size)
{
    size_t const pre_allocated_size = allocator->total_allocated_size();
    char *data = (char *) allocator->allocate(header_size() + sizeof(uint64_t));

    proto::geo_data_t header;
    header.set_magic(SYSTEM_ENDIAN_FLAG);

#define GEO_BASE_DEF_VAR(var_t, var) \
    header.set_##var(g.var());

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    GEO_BASE_DEF_VAR(number_t, arr##_number); \
    if (g.arr##_number() > 0) { \
        arr_t *arr = (arr_t *) allocator->allocate(sizeof(arr_t) * g.arr##_number()); \
        memcpy(arr, g.arr(), sizeof(arr_t) * g.arr##_number()); \
        header.set_##arr((uint64_t) (((intptr_t) arr) - ((intptr_t) data))); \
        header.set_##arr##_crc32(crc32(arr, std::min(g.arr##_number() * sizeof(arr_t), CRC_SIZE))); \
    };

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    auto const str = header.SerializeAsString();
    *((uint64_t *) data) = (uint64_t) htonl(str.size());
    memcpy(data + sizeof(uint64_t), str.data(), str.size());

    if (size)
        *size = allocator->total_allocated_size() - pre_allocated_size;

    return data;
}

geo_data_map_t::geo_data_map_t(geo_data_t const &geo_data, block_allocator_t *allocator)
    : geo_data_map_t()
{
    data_ = serialize(geo_data, allocator, &size_);
    remap();
}

} // namespace geo_base
