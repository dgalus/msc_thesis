#include <iostream>

#include "include/elasticsearch/elasticsearch.h"

int main(int argc, char **argv)
{
    Elasticsearch es{};
	
	//JsonResponse jr = es.search("unsafe", "url", "");
	//std::cout << jr.GetStatusCode() << std::endl;
	//std::cout << jr.GetRawData() << std::endl;
	//rapidjson::StringBuffer buffer;
	//rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	//jr.GetJsonData().Accept(writer);
	//std::cout << buffer.GetString() << std::endl;

	BulkBody bb;
	for(int i = 0; i < 100; i++)
	{
		bb.Add(BulkOperation::Index, "{ \"_index\" : \"test\", \"_type\" : \"seq\" }", "{ \"title\" : \"" + std::to_string(i) + "\" }");
	}

	std::cout << es.bulk(bb.Get()).GetRawData();

    return 0;
}
