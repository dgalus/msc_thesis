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
		rapidjson::Document d;
		d.SetObject();
		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
		
		if(p.pdu()->find_pdu<Tins::EthernetII>())
		{
			rapidjson::Value ethernet(rapidjson::kObjectType);
			
			std::string dst_addr = p.pdu()->rfind_pdu<Tins::EthernetII>().dst_addr().to_string();
			rapidjson::Value dst_addr_val;
			dst_addr_val.SetString(dst_addr.c_str(), dst_addr.length(), allocator);
			ethernet.AddMember("dst_addr", dst_addr_val, allocator);
			
			std::string src_addr = p.pdu()->rfind_pdu<Tins::EthernetII>().src_addr().to_string();
			rapidjson::Value src_addr_val;
			src_addr_val.SetString(src_addr.c_str(), src_addr.length(), allocator);
			ethernet.AddMember("src_addr", src_addr_val, allocator);
			
			d.AddMember(rapidjson::StringRef("ethernet"), ethernet, allocator);

			/*if(p.pdu()->find_pdu<Tins::IP>())
			{
				rapidjson::Value ip(rapidjson::kObjectType);
				
				int version = p.pdu()->rfind_pdu<Tins::IP>().version();
				ip.AddMember(rapidjson::StringRef("version"), version, allocator);
				
				int tot_len = p.pdu()->rfind_pdu<Tins::IP>().tot_len();
				ip.AddMember(rapidjson::StringRef("tot_len"), tot_len, allocator);
				
				Tins::IP::Flags flags = p.pdu()->rfind_pdu<Tins::IP>().flags();
				if(flags == Tins::IP::Flags::FLAG_RESERVED)
					ip.AddMember(rapidjson::StringRef("flags"), rapidjson::StringRef("reserved"), allocator);
				else if(flags == Tins::IP::Flags::DONT_FRAGMENT)
					ip.AddMember(rapidjson::StringRef("flags"), rapidjson::StringRef("dont_fragment"), allocator);
				else if(flags == Tins::IP::Flags::MORE_FRAGMENTS)
					ip.AddMember(rapidjson::StringRef("flags"), rapidjson::StringRef("more_fragments"), allocator);
				else
					ip.AddMember(rapidjson::StringRef("flags"), rapidjson::StringRef(""), allocator);
				
				int ttl = p.pdu()->rfind_pdu<Tins::IP>().ttl();
				ip.AddMember(rapidjson::StringRef("ttl"), ttl, allocator);
				
				int protocol = p.pdu()->rfind_pdu<Tins::IP>().protocol();
				ip.AddMember(rapidjson::StringRef("protocol"), protocol, allocator);
				
				std::string src_addr = p.pdu()->rfind_pdu<Tins::IP>().src_addr().to_string();
				std::string dst_addr = p.pdu()->rfind_pdu<Tins::IP>().dst_addr().to_string();
				ip.AddMember(rapidjson::StringRef("dst_addr"), rapidjson::StringRef(dst_addr.c_str()), allocator);
				ip.AddMember(rapidjson::StringRef("src_addr"), rapidjson::StringRef(src_addr.c_str()), allocator);
				
				d.AddMember(rapidjson::StringRef("ip"), ip, allocator);

				if(p.pdu()->find_pdu<Tins::TCP>())
				{
					// TCP
					p.pdu()->rfind_pdu<Tins::TCP>().sport();
					p.pdu()->rfind_pdu<Tins::TCP>().dport();
					p.pdu()->rfind_pdu<Tins::TCP>().seq();
					p.pdu()->rfind_pdu<Tins::TCP>().ack_seq();
					p.pdu()->rfind_pdu<Tins::TCP>().flags();
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
			{ }*/

			// SIZE
		}
		
		// DEBUG
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer, rapidjson::Document::EncodingType, rapidjson::UTF8<>> writer(buffer);
		d.Accept(writer);
		std::cout << buffer.GetString() << std::endl;
		
		

		// bb.Add(BulkOperation::Index, "{ \"_index\" : \"test\", \"_type\" : \"ip\" }", "{ \"src\" : \"" + p.pdu()->rfind_pdu<Tins::IP>().src_addr().to_string() + "\" }");
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
