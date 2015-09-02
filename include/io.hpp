#pragma once

#include <iostream>

namespace geo_base {

typedef std::ostream output_t;
typedef std::istream input_t;

bool gb_read(int fd, char *ptr, size_t count);
bool gb_write(int fd, char const *ptr, size_t count);

}
