#include "libnetstat.h"

std::vector<std::string> Libnetstat::GetInterfaces()
{
	struct ifaddrs *ifaddr, *ifa;
	std::vector<std::string> interfaceNames;
	
	if(getifaddrs(&ifaddr) == -1)
	{
		throw new GetIfAddrException();
	}
	for(ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
	{
		if(ifa->ifa_addr == NULL)
			continue;
		if(std::find(interfaceNames.begin(), interfaceNames.end(), std::string(ifa->ifa_name)) == interfaceNames.end())
			interfaceNames.push_back(std::string(ifa->ifa_name));
	}
	return interfaceNames;
}

Traffic Libnetstat::GetInterfaceTrafficBytes(std::string& interfaceName)
{
	Traffic traffic;
	std::fstream rx_file("/sys/class/net/" + interfaceName + "/statistics/rx_bytes", std::ios_base::in);
	std::fstream tx_file("/sys/class/net/" + interfaceName + "/statistics/tx_bytes", std::ios_base::in);
	unsigned long long rx;
	unsigned long long tx;
	rx_file >> rx;
	tx_file >> tx;
	traffic.rx = rx;
	traffic.tx = tx;
	return traffic;
}

Traffic Libnetstat::GetInterfaceTrafficPackets(std::string& interfaceName)
{
	Traffic traffic;
	std::fstream rx_file("/sys/class/net/" + interfaceName + "/statistics/rx_packets", std::ios_base::in);
	std::fstream tx_file("/sys/class/net/" + interfaceName + "/statistics/tx_packets", std::ios_base::in);
	unsigned long long rx;
	unsigned long long tx;
	rx_file >> rx;
	tx_file >> tx;
	traffic.rx = rx;
	traffic.tx = tx;
	return traffic;
}

