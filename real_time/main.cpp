#include <iostream>

#include "include/elasticsearch/elasticsearch.h"

int main(int argc, char **argv)
{
    Elasticsearch es{};
	
	JsonResponse jr = es.search("unsafe", "url", "");
	std::cout << jr.GetStatusCode() << std::endl;
	std::cout << jr.GetRawData() << std::endl;
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	jr.GetJsonData().Accept(writer);
	std::cout << buffer.GetString() << std::endl;
    return 0;
}
