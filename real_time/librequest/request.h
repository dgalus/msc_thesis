#ifndef REQUEST_H_
#define REQUEST_H_

#include <boost/asio.hpp>
#include <iostream>

class Request {
public:
	
	
private:
	
};

struct Response {
	int status_code;
	std::map<std::string, std::string> header;
	std::string text;
};

#endif
