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
		log_error() << "geo-base-repeat <count>";
		return -1;
	}
	
	std::string text;

	std::string buffer;
	while (std::getline(std::cin, buffer))
		text.append(buffer).append("\n");

	long long count = atoll(argv[1]);

	while (count == -1 || count > 0) {
		std::cout << text;
		if (count != -1)
			--count;
	}

	return 0;
}
