#include "geo_base_alloc.h"
#include "vector.h"

#include <iostream>

static void usage()
{
	std::cerr << "geo-base-generate <geo.dat>" << std::endl;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	if (argc != 2) {
		usage();
		return -1;
	}

	troll::geo_base_alloc_t base(argv[1]);

	return 0;
}
