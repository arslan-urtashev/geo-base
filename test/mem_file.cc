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
#include <geo_base/util/mem_file.h>

using namespace geo_base;

#define ASSERT_MEM_EQ(file) \
	ASSERT_EQ(-1, file.fd()); \
	ASSERT_EQ(nullptr, file.data()); \
	ASSERT_EQ(0ull, file.size());

#define ASSERT_MEM_NE(file) \
	ASSERT_NE(-1, file.fd()); \
	ASSERT_NE(nullptr, file.data()); \
	ASSERT_NE(0ull, file.size());

TEST(mem_file_t, read_open)
{
	mem_file_t file;
	ASSERT_MEM_EQ(file);

	file.read_open("test/andorra-latest.osm.pbf");
	ASSERT_MEM_NE(file);
}

TEST(mem_file_t, read_open_wrong_file)
{
	mem_file_t file;
	ASSERT_MEM_EQ(file);
	EXPECT_THROW(file.read_open("wrong_file"), exception_t);
}

TEST(mem_file_t, move)
{
	mem_file_t file;
	ASSERT_MEM_EQ(file);

	{
		mem_file_t file1;
		file1.read_open("test/andorra-latest.osm.pbf");
		ASSERT_MEM_NE(file1);

		file = std::move(file1);

		ASSERT_MEM_EQ(file1);
		ASSERT_MEM_NE(file);
	}
}

TEST(mem_file_t, read_mmap)
{
	static std::string const FILENAME = "mem_file_test.txt";
	static std::string const TEXT = "Hello, world!\0";

	{
		file_t file;
		file.read_write_open(FILENAME.data());
		ASSERT_EQ((int) TEXT.size(), write(file.fd(), TEXT.data(), TEXT.size()));
	}

	{
		mem_file_t mem_file;
		mem_file.read_open(FILENAME.data());
		ASSERT_STREQ(TEXT.data(), (char const *) mem_file.data());
	}

	ASSERT_EQ(0, remove(FILENAME.data()));
}
