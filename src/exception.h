// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//   
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//           
//   The above copyright notice and this permission notice shall be included
//   in all copies or substantial portions of the Software.
//              
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef GEO_BASE_EXCEPTION_H_
#define GEO_BASE_EXCEPTION_H_

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <exception>

namespace geo_base {

class Exception : public std::exception {
public:
  static const size_t MESSAGE_LIMIT = 128;

  template<typename... Args>
  Exception(const char* fmt, Args... args) noexcept {
    snprintf(message, MESSAGE_LIMIT, fmt, args...);
  }

  virtual const char* what() const noexcept {
    return message;
  }
  
private:
  char message[MESSAGE_LIMIT];
};

} // namespace geo_base

#endif // GEO_BASE_EXCEPTION_H_
