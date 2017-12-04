#ifndef RESPONSE_H_
#define RESPONSE_H_

#include <string>

#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"

class JsonResponse {
public:
	JsonResponse(int status_code, std::string raw_data);
	int& GetStatusCode();
	std::string& GetRawData();
	rapidjson::Document& GetJsonData();
private:
	int status_code;
	std::string raw_data;
	rapidjson::Document json_data;
};

#endif
