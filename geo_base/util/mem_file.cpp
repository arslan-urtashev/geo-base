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

#include <geo_base/util/mem_file.h>

#include <errno.h>
#include <sys/stat.h>

namespace geo_base {

void mem_file_t::read_open(char const *path)
{
    file_t::read_open(path);

    struct stat buf;
    if (fstat(fd(), &buf) < 0)
        throw exception_t("Unable fstat %s: %s", path, strerror(errno));

    void *memory = mmap(nullptr, buf.st_size, PROT_READ, MAP_SHARED, fd(), 0);
    if (memory == MAP_FAILED)
        throw exception_t("Unable mmap file %s: %s", path, strerror(errno));

    mem_guard_ = mem_guard_t(memory, buf.st_size);
}

void mem_file_t::read_write_open(char const *path, size_t mmap_size)
{
    file_t::read_write_open(path);

    void *memory = mmap(nullptr, mmap_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd(), 0);
    if (memory == MAP_FAILED)
        throw exception_t("Unable mmap file %s: %s", path, strerror(errno));

    mem_guard_ = mem_guard_t(memory, mmap_size);
}

} // namespace geo_base
