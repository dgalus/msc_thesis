#include "utils.h"

namespace Librequest {
namespace util {

std::vector<std::string> split(const std::string& text, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream stream(text);
	std::string i;
	while(std::getline(stream, i, delimiter))
	{
		tokens.push_back(i);
	}
	return tokens;
}
	
}
}
