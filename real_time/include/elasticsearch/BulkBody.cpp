#include "BulkBody.h"

BulkBody::BulkBody()
{
    this->bulkString = "";
}

BulkBody& BulkBody::Add(BulkOperation &bulkOperation, std::string &&jsonBody, std::string&& source) {
    std::replace(jsonBody.begin(), jsonBody.end(), '\n', ' ');
    if(!source.empty())
        std::replace(source.begin(), source.end(), '\n', ' ');
    switch (bulkOperation)
    {
        case BulkOperation::Create:
            if(source.empty())
                throw BulkException("Create operation needs source.");
            break;
        case BulkOperation::Delete:
            break;
        case BulkOperation::Index:
            if(source.empty())
                throw BulkException("Index operation needs source.");
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