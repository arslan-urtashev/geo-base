#pragma once

#include <zlib.h>

namespace geo_base {

class zlib_t {
public:
	size_t unpack(char const *dat, size_t dat_size, char *unpack, size_t unpack_size) const
	{
		z_stream z;
		z.next_in = (unsigned char*) dat;
		z.avail_in = dat_size;
		z.next_out = (unsigned char*) unpack;
		z.avail_out = unpack_size;
		z.zalloc = Z_NULL;
		z.zfree = Z_NULL;
		z.opaque = Z_NULL;

		if(inflateInit(&z) != Z_OK)
			throw exception_t("%s", "Can't init zlib");

		if(inflate(&z, Z_FINISH) != Z_STREAM_END)
			throw exception_t("%s", "Can't inflate zlib");

		if(inflateEnd(&z) != Z_OK)
			throw exception_t("%s", "Can't deinit zlib");

		return z.total_out;
	}
};

}
