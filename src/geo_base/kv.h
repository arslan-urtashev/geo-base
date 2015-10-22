// copyright (c) 2015 urtashev arslan. all rights reserved.
// contacts: <urtashev@gmail.com>
//
// permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "software"), to deal in the software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the software, and to permit persons to whom the
// software is furnished to do so, subject to the following conditions:
//
//   the above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the software.
//
// the software is provided "as is", without warranty of any kind, express or implied, including
// but not limited to the warranties of merchantability, fitness for a particular purpose and
// noninfringement. in no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract, tort or otherwise, arising from,
// out of or in connection with the software or the use or other dealings in the software.

#pragma once

#include "typedef.h"

namespace geo_base {

// Key-Value. k and v is offsets on blobs in geographical data blobs array. See geo_data.h
// for details.
struct kv_t {
	count_t k;
	count_t v;
};

} // namespace geo_base
