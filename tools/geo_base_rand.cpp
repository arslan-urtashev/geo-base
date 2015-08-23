#include "location.h"
#include "log.h"

#include <iomanip>
#include <iostream>

using namespace troll;

int main(int argc, char *argv[])
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(6);
	
	if (argc != 2) {
		log_error("geo-base-rand") << "geo-base-rand <count>";
		return -1;
	}

	srand(337);

	long long count = atoll(argv[1]);
	while (count == -1 || count > 0) {
		location_t l;
		
		l.lon = convert_to_double(rand() % convert_to_coordinate(360.0) - convert_to_coordinate(180.0));
		l.lat = convert_to_double(rand() % convert_to_coordinate(180.0) - convert_to_coordinate(90.0));
		
		std::cout << l.lon << ' ' << l.lat << std::endl;
		
		if (count != -1)
			--count;
	}

	return 0;
}
