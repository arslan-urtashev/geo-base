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

#include <arpa/inet.h>
#include <stdint.h>
#include <type_traits>

// Workaround missing builtin functions in g++ < 5.0
#if __GNUG__ && __GNUC__ < 5
#	define IS_TRIVIALLY_COPYABLE(T) __has_trivial_copy(T)
#	define IS_TRIVIALLY_DESTRUCTIBLE(T) __has_trivial_destructor(T)
#	define IS_TRIVIALLY_CONSTRUCTIBLE(T) __has_trivial_constructor(T)
#else
#	define IS_TRIVIALLY_COPYABLE(T) std::is_trivially_copyable<T>::value
#	define IS_TRIVIALLY_DESTRUCTIBLE(T) std::is_trivially_destructible<T>::value
#	define IS_TRIVIALLY_CONSTRUCTIBLE(T) std::is_trivially_constructible<T>::value
#endif

namespace geo_base {

uint32_t const SYSTEM_ENDIAN_FLAG = htonl(337);

} // namespace geo_base
