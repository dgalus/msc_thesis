#include "libnetstat.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(int argc, char *argv[])
{
	auto ret = Libnetstat::GetInterfaces();
	for(auto& r : ret)
	{
		std::cout << r << std::endl;
	}
	return 0;
}
