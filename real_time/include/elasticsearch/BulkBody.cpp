#include "BulkBody.h"
#include "../rapidjson/document.h"

BulkBody::BulkBody()
{
    this->bulkString = "";
    this->counter = 0;
}

BulkBody& BulkBody::Add(BulkOperation bulkOperation, std::string &&jsonBody, std::string jsonSource) {
    std::replace(jsonBody.begin(), jsonBody.end(), '\n', ' ');
    rapidjson::Document d;
    rapidjson::Document s;
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
            if(jsonSource.empty())
                throw BulkException("Update operation needs source.");
            s.Parse(jsonSource.c_str());
            if(d.HasMember("_index") && d.HasMember("_type") && d.HasMember("_id") &&
                    (s.HasMember("script") || s.HasMember("doc") || s.HasMember("upsert")
                    || s.HasMember("doc_as_upsert") || s.HasMember("_source")))
                this->bulkString += "{ \"update\" : " + jsonBody + " }\n" + jsonSource + "\n";
            else
                throw BulkException("Update operation needs _index, _type and _id. Source needs one of (script, doc, upsert, doc_as_upsert, _source).");
            break;
        default:
            break;
    }
    counter++;
    return *this;
}

std::string BulkBody::Get() 
{
    return this->bulkString;
}

void BulkBody::Clear()
{
    this->bulkString = "";
    counter = 0;
}

uint32_t BulkBody::Count()
{
    return this->counter;
}