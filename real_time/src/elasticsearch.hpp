#ifndef ELASTICSEARCH_HPP_
#define ELASTICSEARCH_HPP_

#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <tins/tins.h>

class Elasticsearch {
public:
    Elasticsearch();
    Elasticsearch(std::string host, uint16_t port);
    Elasticsearch(std::string host, std::string username, std::string password, uint16_t port);
    void SetHost(std::string host);
    void SetPort(uint16_t port);
    void SetUsername(std::string username);
    void SetPassword(std::string password);

private:
    std::string host;
    std::string username;
    std::string password;
    uint16_t port;
};

#endif