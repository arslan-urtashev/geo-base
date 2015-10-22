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

#include "geo_data.h"
#include "proto/geo_data.pb.h"
#include "util/exception.h"

namespace geo_base {

void geo_data_t::save(output_stream_t *out)
{
	proto::geo_data_t message;

#define GEO_BASE_DEF_VAR(var_t, var) \
	message.set_##var(var());

#define GEO_BASE_DEF_ARR(arr_t, arr) \
	message.mutable_##arr()->set_count(arr##_count()); \

	GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

	std::string const &data = message.SerializeAsString();
	if (!out->write(data.data(), data.size()))
		throw exception_t("Can't write serialized data!");

#define GEO_BASE_DEF_VAR(var_t, var) \
	// undef

#define GEO_BASE_DEF_ARR(arr_t, arr) \
	if (!out->write((char const *) arr(), sizeof(arr_t) * arr##_count())) \
		throw exception_t("Can't write " #arr "!");

	GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR
}

} // namespace geo_base
