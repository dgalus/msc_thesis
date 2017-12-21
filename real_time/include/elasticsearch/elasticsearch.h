#ifndef ELASTICSEARCH_H_
#define ELASTICSEARCH_H_

#include <exception>
#include <string>
#include <vector>
#include <tins/tins.h>

#include "json_response.h"
#include "../cpr/cpr.h"
#include "BulkBody.h"

class Elasticsearch {
public:
    Elasticsearch();
    Elasticsearch(std::string host, uint16_t port);
    Elasticsearch(std::string host, std::string username, std::string password, uint16_t port);
    void SetHost(std::string host);
    void SetPort(uint16_t port);
    void SetUsername(std::string username);
    void SetPassword(std::string password);
	JsonResponse search(std::string index, std::string document_type, std::string&& body);
	JsonResponse bulk(std::string&& body);

private:
    std::string host;
    std::string username;
    std::string password;
    uint16_t port;
};

struct ElasticsearchResponseException : public std::exception
{
private:
    int _status;
    std::string _body;
public:
    ElasticsearchResponseException(const int status_code, const std::string& error) : std::exception()
    {
        _status = status_code;
        _body = error;
    }
    virtual const char * what() const throw ()
    {
        return "Elasticsearch exception";
    }
    int status()
    {
        return _status;
    }
    std::string body()
    {
        return _body;
    }
};

#endif
