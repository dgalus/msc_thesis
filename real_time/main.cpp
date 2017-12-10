#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <tins/tins.h>

#include "include/elasticsearch/elasticsearch.h"


std::vector<Tins::Packet> vt;
Elasticsearch es{};
std::mutex m;

void save()
{
	BulkBody bb;
	std::lock_guard<std::mutex> lock(m);

	for(auto p : vt)
	{
		if(p.pdu()->find_pdu<Tins::EthernetII>())
		{
			rapidjson::Document d;
			d.SetObject();
			rapidjson::Value ethernet(rapidjson::kObjectType);
			std::string dst_addr = p.pdu()->rfind_pdu<Tins::EthernetII>().dst_addr().to_string();
			std::string src_addr = p.pdu()->rfind_pdu<Tins::EthernetII>().src_addr().to_string();
			ethernet.AddMember(rapidjson::StringRef("dst_addr"),
				rapidjson::StringRef(dst_addr.c_str()),
				d.GetAllocator());
			ethernet.AddMember(rapidjson::StringRef("src_addr"),
				rapidjson::StringRef(src_addr.c_str()),
				d.GetAllocator());
			d.AddMember(rapidjson::StringRef("ethernet"), ethernet, d.GetAllocator());
			
			// DEBUG
			std::cout << "In debug" <<std::endl;
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer, rapidjson::Document::EncodingType, rapidjson::ASCII<>> writer(buffer);
			d.Accept(writer);
			std::cout << buffer.GetString() << std::endl;



			// ETH
			if(p.pdu()->find_pdu<Tins::IP>())
			{
				// IP
				/*p.pdu()->rfind_pdu<Tins::IP>().version();
				p.pdu()->rfind_pdu<Tins::IP>().tot_len();
				p.pdu()->rfind_pdu<Tins::IP>().flags();
				p.pdu()->rfind_pdu<Tins::IP>().ttl();
				p.pdu()->rfind_pdu<Tins::IP>().protocol();
				p.pdu()->rfind_pdu<Tins::IP>().src_addr();
				p.pdu()->rfind_pdu<Tins::IP>().dst_addr();*/

				if(p.pdu()->find_pdu<Tins::TCP>())
				{
					// TCP
					/*p.pdu()->rfind_pdu<Tins::TCP>().sport();
					p.pdu()->rfind_pdu<Tins::TCP>().dport();
					p.pdu()->rfind_pdu<Tins::TCP>().seq();
					p.pdu()->rfind_pdu<Tins::TCP>().ack_seq();
					p.pdu()->rfind_pdu<Tins::TCP>().flags();*/
				}
				else if(p.pdu()->find_pdu<Tins::UDP>())
				{
					// UDP
				}
				else
				{ }
			}
			else if(p.pdu()->find_pdu<Tins::ICMP>())
			{

			}
			else
			{ }

			// SIZE
		}
		if(p.pdu()->find_pdu<Tins::IP>()) {
			bb.Add(BulkOperation::Index, "{ \"_index\" : \"test\", \"_type\" : \"ip\" }",
				   "{ \"src\" : \"" + p.pdu()->rfind_pdu<Tins::IP>().src_addr().to_string() + "\" }");
		}
	}
	//std::cout << es.bulk(bb.Get()).GetRawData();
	vt.clear();
}

bool doo(Tins::Packet& packet)
{
	vt.push_back(packet);
	if(vt.size() == 10)
	{
		save();
	}
	return true;
}

int main(int argc, char **argv)
{

	Tins::Sniffer sniffer("eno1");
	sniffer.sniff_loop(doo);
	
	//JsonResponse jr = es.search("unsafe", "url", "");
	//std::cout << jr.GetStatusCode() << std::endl;
	//std::cout << jr.GetRawData() << std::endl;
	//rapidjson::StringBuffer buffer;
	//rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	//jr.GetJsonData().Accept(writer);
	//std::cout << buffer.GetString() << std::endl;

	/*BulkBody bb;
	for(int i = 0; i < 100; i++)
	{
		bb.Add(BulkOperation::Index, "{ \"_index\" : \"test\", \"_type\" : \"seq\" }", "{ \"title\" : \"" + std::to_string(i) + "\" }");
	}*/

    return 0;
}
