#ifndef BULKBODY_H_
#define BULKBODY_H_

#include <algorithm>
#include <exception>
#include <string>
#include <vector>

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
    BulkBody& Add(BulkOperation& bulkOperation, std::string&& jsonBody, std::string&& source);
    std::string& Get();
private:
    std::string bulkString;
};




#endif
