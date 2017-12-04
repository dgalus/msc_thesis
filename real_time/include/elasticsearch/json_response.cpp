#include "json_response.h"

JsonResponse::JsonResponse(int status_code, std::string raw_data)
{
	this->status_code = status_code;
	this->raw_data = raw_data;
	this->json_data.Parse(raw_data.c_str());
}

int & JsonResponse::GetStatusCode()
{
	return this->status_code;
}

std::string & JsonResponse::GetRawData()
{
	return this->raw_data;
}

rapidjson::Document & JsonResponse::GetJsonData()
{
	return this->json_data;
}



