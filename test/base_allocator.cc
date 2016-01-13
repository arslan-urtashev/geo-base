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

#include <gmock/gmock.h>
#include <geo_base/util/base_allocator.h>

using namespace geo_base;

TEST(base_allocator, base_allocator)
{
	static std::string const FILENAME = "base_allocator_test.txt";
	static std::string const TEXT = "Hello, base_allocator_t world!\0";

	{
		base_allocator_t allocator(FILENAME.data());

		char *ptr = (char *) allocator.allocate(TEXT.size());
		ASSERT_NE(nullptr, ptr);

		std::copy(TEXT.begin(), TEXT.end(), ptr);
	}

	{
		mem_file_t mem_file;
		mem_file.read_open(FILENAME.data());

		ASSERT_EQ(align_memory(TEXT.size()), mem_file.size());
		ASSERT_STREQ(TEXT.data(), (char const *) mem_file.data());
	}

	ASSERT_EQ(0, remove(FILENAME.data()));
}
