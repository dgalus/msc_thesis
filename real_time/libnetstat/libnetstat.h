#ifndef LIBNETSTAT_H_
#define LIBNETSTAT_H_

#include <string>
#include <vector>

class Libnetstat {
public:
	static std::vector<std::string> GetInterfaces();
};

#endif
