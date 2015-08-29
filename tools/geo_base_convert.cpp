#include <stdint.h>
#include <netinet/in.h>
#include <zlib.h>

#include "log.h"

using namespace geo_base;
using namespace protobuf;
using namespace protobuf::io;

class pbf_parser_t {
public:
	pbf_parser_t(char const *path)
	{
		fd_base.rd(path);
		input = new FileInputStream(fd_base.fd());
	}

	void parse()
	{
	}

private:
	fd_base_t fd_base;
};

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	
	if (argc < 2) {
		log_error("geo-base-convert") << "geo-base-convert <OSM.pbf>";
		return -1;
	}

	fd_base_t fd_base;
	fd_base.rd(argv[1]);

	return 0;
}
