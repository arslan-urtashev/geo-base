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

#ifndef GEO_BASE_IO_H_
#define GEO_BASE_IO_H_

#include <iostream>

namespace geo_base {

// input/output stream typedefs, because standard streams too slow.
// Streams can be rewritten in the future.
typedef std::istream InputStream;
typedef std::ostream OutputStream;

// Read is a read wrapper, guaranteeing full data read (standard read does
// not guarantee a full read). Throws exception if read returns error.
bool Read(int fd, char *ptr, size_t count);

// Write is a write wrapper, guaranteeing full data write (standard write dows
// not guarantee a full write). Throws exception if write returns error.
bool Write(int fd, const char* ptr, size_t count);

} // namespace geo_base

#endif // GEO_BASE_IO_H_
