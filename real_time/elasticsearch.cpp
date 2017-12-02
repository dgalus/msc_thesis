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



