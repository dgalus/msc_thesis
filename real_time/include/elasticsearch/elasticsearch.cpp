#include "elasticsearch.h"

Elasticsearch::Elasticsearch()
{
	this->port = 9200;
	this->host = "127.0.0.1";
	this->username = "";
	this->password = "";
}

Elasticsearch::Elasticsearch(std::string host, uint16_t port)
{
	this->port = port;
	this->host = host;
	this->username = "";
	this->password = "";
}

Elasticsearch::Elasticsearch(std::string host, std::string username, std::string password, uint16_t port)
{
	this->host = host;
	this->username = username;
	this->password = password;
	this->port = port;
}

void Elasticsearch::SetHost(std::string host)
{
	this->host = host;
}

void Elasticsearch::SetPassword(std::string password)
{
	this->password = password;
}

void Elasticsearch::SetPort(uint16_t port)
{
	this->port = port;
}

void Elasticsearch::SetUsername(std::string username)
{
	this->username = username;
}

JsonResponse Elasticsearch::search(std::string index, std::string document_type, std::string&& body)
{
	std::string m_url = "http://" + this->host + ":" + std::to_string(this->port);
    cpr::Url url;
    if (index.empty() && document_type.empty()) {
        url = cpr::Url{m_url + "/_search"};
    } else if (document_type.empty()) {
        url = cpr::Url{m_url + "/" + index + "/_search"};
    } else {
        url = cpr::Url{m_url + "/" + index + "/" + document_type + "/_search"};
    }
    cpr::Response request = cpr::Get(url, cpr::Body{body}, cpr::Header{{"Content-type", "application/json"}});
	JsonResponse r(request.status_code, request.text);
	return r;
}

JsonResponse Elasticsearch::bulk(std::string&& body)
{
	return NULL;
}


