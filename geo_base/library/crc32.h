#include <algorithm>

namespace geo_base {

uint32_t crc32(uint32_t crc, void const *buf, size_t size);

uint32_t crc32(void const *buf, size_t size);

} // namespace geo_base
