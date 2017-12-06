#include "BulkBody.h"
#include "../rapidjson/document.h"

BulkBody::BulkBody()
{
    this->bulkString = "";
}

BulkBody& BulkBody::Add(BulkOperation &bulkOperation, std::string &&jsonBody, std::string&& jsonSource) {
    std::replace(jsonBody.begin(), jsonBody.end(), '\n', ' ');
    rapidjson::Document d;
    d.Parse(jsonBody.c_str());
    if(!jsonSource.empty())
        std::replace(jsonSource.begin(), jsonSource.end(), '\n', ' ');
    switch (bulkOperation)
    {
        case BulkOperation::Create:
            if(jsonSource.empty())
                throw BulkException("Create operation needs source.");
            if(d.HasMember("_index") && d.HasMember("_type"))
                this->bulkString += "{ \"create\" : " + jsonBody + " }\n" + jsonSource + "\n";
            else
                throw BulkException("Create operation needs at least _index and _type.");
            break;
        case BulkOperation::Delete:
            if(d.HasMember("_index") && d.HasMember("_type") && d.HasMember("_id"))
                this->bulkString += "{ \"delete\" : " + jsonBody + "}\n";
            else
                throw BulkException("Delete operation needs _index, _type and _id.");
            break;
        case BulkOperation::Index:
            if(jsonSource.empty())
                throw BulkException("Index operation needs source.");
            if(d.HasMember("_index") && d.HasMember("_type"))
                this->bulkString += "{ \"index\" : " + jsonBody + " }\n" + jsonSource + "\n";
            else
                throw BulkException("Create operation needs at least _index and _type.");
            break;
        case BulkOperation::Update:
            break;
        default:
            break;
    }
    return *this;
}

std::string& BulkBody::Get() {

    return this->bulkString;
}