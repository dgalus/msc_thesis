#ifndef LIBNETSTAT_H_
#define LIBNETSTAT_H_

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <ifaddrs.h>
#include <linux/if_link.h>

struct Traffic {
	unsigned long long rx;
	unsigned long long tx;
};

class Libnetstat {
public:
	static std::vector<std::string> GetInterfaces();
	static Traffic GetInterfaceTrafficBytes(std::string& interfaceName);
	static Traffic GetInterfaceTrafficPackets(std::string& interfaceName);
};

struct GetIfAddrException : public std::runtime_error
{
	GetIfAddrException()
		: std::runtime_error("GetIfAddr returned -1.")
	{}
};

#endif
