#ifndef BULKBODY_H_
#define BULKBODY_H_

#include <algorithm>
#include <exception>
#include <string>
#include <vector>
#include <iostream>

#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/writer.h"

enum class BulkOperation {
    Index,
    Create,
    Update,
    Delete
};

class BulkException : public std::exception
{
public:
    explicit BulkException(const char* message)
            : msg_(message) {}

    explicit BulkException(const std::string& message)
            : msg_(message) {}

    virtual ~BulkException() throw ()
    {}

    virtual const char* what() const throw()
    {
        return msg_.c_str();
    }
protected:
    std::string msg_;
};

class BulkBody {
public:
    BulkBody();
    BulkBody& Add(BulkOperation bulkOperation, std::string&& jsonBody, std::string jsonSource);
    std::string Get();
    void Clear();
    uint32_t Count();
private:
    std::string bulkString;
    uint32_t counter;
};




#endif
