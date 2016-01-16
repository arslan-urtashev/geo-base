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

#include <geo_base/util/io_stream.h>

namespace geo_base {

class file_output_stream_t : public output_stream_t {
public:
	file_output_stream_t()
		: fd_(-1)
	{
	}

	explicit file_output_stream_t(int fd)
		: fd_(fd)
	{
	}

	file_output_stream_t(file_output_stream_t const &s)
		: fd_(s.fd_)
	{
	}

	bool write(char const *ptr, size_t count) override;

private:
	int fd_;
};

class file_input_stream_t : public input_stream_t {
public:
	file_input_stream_t()
		: fd_(-1)
	{
	}

	explicit file_input_stream_t(int fd)
		: fd_(fd)
	{
	}

	file_input_stream_t(file_input_stream_t const &s)
		: fd_(s.fd_)
	{
	}

	bool read(char *ptr, size_t count) override;

private:
	int fd_;
};

} // namespace geo_base
