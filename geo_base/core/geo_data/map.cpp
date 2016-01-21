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

#include <geo_base/lib/system.h>
#include <geo_base/core/geo_data/map.h>

namespace geo_base {

geo_data_map_t::geo_data_map_t()
    : data_(nullptr)
    , size_(0)
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

static uint64_t header_space()
{
    uint64_t space = 0;

    // Header size.
    space += sizeof(uint64_t);

    // Endian flag.
    space += sizeof(SYSTEM_ENDIAN_FLAG);

#define GEO_BASE_DEF_VAR(var_t, var) \
    space += sizeof(var_t);

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    space += sizeof(number_t); \
    space += sizeof(uint64_t);

    GEO_BASE_DEF_GEO_DATA;

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    return space;
}

template<typename value_t>
static void serialize_value(char **ptr, value_t const &v)
{
    *((value_t *) *ptr) = v;
    *ptr += sizeof(value_t);
}

template<typename array_t>
static void serialize_array(char const *begin, char **ptr, array_t const *arr,
    number_t number, block_allocator_t *allocator)
{
    serialize_value(ptr, number);
    array_t *buf = (array_t *) allocator->allocate(sizeof(array_t) * number);
    std::copy_n(arr, number, buf);
    serialize_value(ptr, ((uint64_t) buf) - ((uint64_t) begin));
}

static char const *serialize(geo_data_t const &geo_data,
    block_allocator_t *allocator, size_t *size)
{
    size_t const pre_allocated_size = allocator->total_allocated_size();
    char *begin = (char *) allocator->allocate(header_space()), *ptr = begin;

    serialize_value(&ptr, SYSTEM_ENDIAN_FLAG);
    serialize_value(&ptr, header_space());

#define GEO_BASE_DEF_VAR(var_t, var) \
    serialize_value(&ptr, geo_data.var());

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    serialize_array(begin, &ptr, geo_data.arr(), geo_data.arr##_number(), allocator);

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    if (size)
        *size = allocator->total_allocated_size() - pre_allocated_size;

    return begin;
}

template<typename value_t>
static bool deserialize_value(char const *end, char const **ptr, value_t *value)
{
    if (*ptr + sizeof(value_t) < end) {
        *value = *((value_t const *) *ptr);
        *ptr += sizeof(value_t);
        return true;
    }
    return false;
}

template<typename array_t>
static bool deserialize_array(char const *begin, char const *end, char const **ptr,
    array_t const **arr, number_t *number)
{
    if (!deserialize_value(end, ptr, number))
        return false;

    uint64_t offset = 0;
    if (!deserialize_value(end, ptr, &offset)) {
        *number = 0;
        return false;
    }

    *arr = (array_t const *) (begin + offset);

    return true;
}

void geo_data_map_t::remap()
{
    char const *ptr = data_;

    uint32_t endian_flag = 0;
    deserialize_value(data_ + size_, &ptr, &endian_flag);

    if (endian_flag != SYSTEM_ENDIAN_FLAG)
        throw exception_t("Not compatible flag in serialized data");

    uint64_t header_space = 0;
    deserialize_value(data_ + size_, &ptr, &header_space);

    char const *end = data_ + header_space;

#define GEO_BASE_DEF_VAR(var_t, var) \
    deserialize_value(end, &ptr, &var##_);

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    deserialize_array(data_, end, &ptr, &arr##_, &arr##_number_); \
    if ((uint64_t) (arr##_ + arr##_number_) > (uint64_t) (data_ + size_)) \
        throw exception_t("%s too big", #arr);

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    if (version() != GEO_DATA_CURRENT_VERSION)
        throw exception_t("Unable use version %llu (current version is %llu)",
            version(), GEO_DATA_CURRENT_VERSION);
}

geo_data_map_t::geo_data_map_t(geo_data_t const &geo_data, block_allocator_t *allocator)
    : geo_data_map_t()
{
    data_ = serialize(geo_data, allocator, &size_);
    remap();
}

} // namespace geo_base
