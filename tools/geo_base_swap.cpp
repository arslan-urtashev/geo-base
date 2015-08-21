#include "location.h"

#include <iomanip>
#include <iostream>
#include <string>

int main()
{
	std::ios_base::sync_with_stdio(false);

	std::string a, b;
	while (std::cin >> a >> b)
		std::cout << b << ' ' << a << '\n';

	return 0;
}
