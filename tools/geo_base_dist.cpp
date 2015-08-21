#include "location.h"

#include <iomanip>
#include <iostream>

using namespace troll;

int main()
{
	std::ios_base::sync_with_stdio(false);
	std::cout << std::fixed << std::setprecision(2);
	
	location_t a, b;
	while (std::cin >> a.lon >> a.lat >> b.lon >> b.lat)
		std::cout << a.dist_to(b) << std::endl;

	return 0;
}
