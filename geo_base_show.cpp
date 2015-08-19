#include "geo_base.h"

#include <iostream>

static void usage()
{
	std::cerr << "geo-base-show <geodata.dat>" << std::endl;
}

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);

	if (argc != 2) {
		usage();
		return -1;
	}

	troll::geo_base_t geo_base(argv[1]);
	geo_base.show(std::cout);

	return 0;
}
