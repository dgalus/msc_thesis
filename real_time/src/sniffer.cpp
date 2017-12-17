#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <tins/tins.h>
#include <time.h>

#include "../include/elasticsearch/elasticsearch.h"


std::vector<Tins::Packet> vt;
Elasticsearch es{};
std::mutex m;

std::string getCurrentDateTime()
{
	time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return std::string(buf);
}

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
			
			d.AddMember("ethernet", ethernet, allocator);

			if(p.pdu()->find_pdu<Tins::IP>())
			{
				rapidjson::Value ip(rapidjson::kObjectType);
				
				int version = p.pdu()->rfind_pdu<Tins::IP>().version();
				rapidjson::Value version_val;
				version_val.SetInt(version);
				ip.AddMember("version", version_val, allocator);
				
				int tot_len = p.pdu()->rfind_pdu<Tins::IP>().tot_len();
				rapidjson::Value tot_len_val;
				tot_len_val.SetInt(tot_len);
				ip.AddMember("tot_len", tot_len_val, allocator);
				
				Tins::IP::Flags flags = p.pdu()->rfind_pdu<Tins::IP>().flags();
				rapidjson::Value flags_val;
				if(flags == Tins::IP::Flags::FLAG_RESERVED)
					flags_val.SetString("reserved");
				else if(flags == Tins::IP::Flags::DONT_FRAGMENT)
					flags_val.SetString("dont_fragment");
				else if(flags == Tins::IP::Flags::MORE_FRAGMENTS)
					flags_val.SetString("more_fragments");
				else
					flags_val.SetString("");
				ip.AddMember("flags", flags_val, allocator);
				
				int ttl = p.pdu()->rfind_pdu<Tins::IP>().ttl();
				rapidjson::Value ttl_val;
				ttl_val.SetInt(ttl);
				ip.AddMember("ttl", ttl_val, allocator);
				
				int protocol = p.pdu()->rfind_pdu<Tins::IP>().protocol();
				rapidjson::Value protocol_val;
				protocol_val.SetInt(protocol);
				ip.AddMember("protocol", protocol_val, allocator);
				
				std::string dst_addr = p.pdu()->rfind_pdu<Tins::IP>().dst_addr().to_string();
				rapidjson::Value dst_addr_val;
				dst_addr_val.SetString(dst_addr.c_str(), dst_addr.length(), allocator);
				ip.AddMember("dst_addr", dst_addr_val, allocator);
				
				std::string src_addr = p.pdu()->rfind_pdu<Tins::IP>().src_addr().to_string();
				rapidjson::Value src_addr_val;
				src_addr_val.SetString(src_addr.c_str(), src_addr.length(), allocator);
				ip.AddMember("src_addr", src_addr_val, allocator);
				
				d.AddMember("ip", ip, allocator);

				if(p.pdu()->find_pdu<Tins::TCP>())
				{
					rapidjson::Value tcp(rapidjson::kObjectType);

					int sport = p.pdu()->rfind_pdu<Tins::TCP>().sport();
					rapidjson::Value sport_val;
					sport_val.SetInt(sport);
					tcp.AddMember("sport", sport_val, allocator);
					
					int dport = p.pdu()->rfind_pdu<Tins::TCP>().dport();
					rapidjson::Value dport_val;
					dport_val.SetInt(dport);
					tcp.AddMember("dport", dport_val, allocator);
					
					unsigned int seq = p.pdu()->rfind_pdu<Tins::TCP>().seq();
					rapidjson::Value seq_val;
					seq_val.SetUint(seq);
					tcp.AddMember("seq", seq_val, allocator);
					
					unsigned int seq_ack = p.pdu()->rfind_pdu<Tins::TCP>().ack_seq();
					rapidjson::Value seq_ack_val;
					seq_ack_val.SetUint(seq_ack);
					tcp.AddMember("seq_ack", seq_ack_val, allocator);
					
					int flags = p.pdu()->rfind_pdu<Tins::TCP>().flags();
					rapidjson::Value flags_val;
					flags_val.SetInt(flags);
					tcp.AddMember("flags", flags_val, allocator);
					
					d.AddMember("tcp", tcp, allocator);
				}
				else if(p.pdu()->find_pdu<Tins::UDP>())
				{
					rapidjson::Value udp(rapidjson::kObjectType);
					
					int dport = p.pdu()->rfind_pdu<Tins::UDP>().dport();
					rapidjson::Value dport_val;
					dport_val.SetInt(dport);
					udp.AddMember("dport", dport_val, allocator);
					
					int sport = p.pdu()->rfind_pdu<Tins::UDP>().sport();
					rapidjson::Value sport_val;
					sport_val.SetInt(sport);
					udp.AddMember("sport", sport_val, allocator);
					
					if(p.pdu()->find_pdu<Tins::DHCP>())
					{
						std::vector<Tins::IPv4Address> dns_tins = p.pdu()->rfind_pdu<Tins::DHCP>().domain_name_servers(); 
						std::vector<std::string> dns;
						rapidjson::Value dns_val(rapidjson::kArrayType);
						for(auto d : dns_tins)
						{
							dns_val.PushBack(d.to_string().c_str(), d.to_string().length(), allocator);
							dns.push_back(d.to_string());
						}

						p.pdu()->rfind_pdu<Tins::DHCP>().subnet_mask();
						p.pdu()->rfind_pdu<Tins::DHCP>().routers(); //vector
						p.pdu()->rfind_pdu<Tins::DHCP>().requested_ip();
					}

					d.AddMember("udp", udp, allocator);
				}
				else if(p.pdu()->find_pdu<Tins::ICMP>())
				{
					rapidjson::Value icmp(rapidjson::kObjectType);
					
					Tins::ICMP::Flags flags = p.pdu()->rfind_pdu<Tins::ICMP>().type();
					rapidjson::Value flags_val;
					if(flags == Tins::ICMP::Flags::ADDRESS_MASK_REPLY)
						flags_val.SetString("address_mask_reply");
					else if(flags == Tins::ICMP::Flags::ADDRESS_MASK_REQUEST)
						flags_val.SetString("address_mask_request");
					else if(flags == Tins::ICMP::Flags::DEST_UNREACHABLE)
						flags_val.SetString("dest_unreachable");
					else if(flags == Tins::ICMP::Flags::ECHO_REPLY)
						flags_val.SetString("echo_reply");
					else if(flags == Tins::ICMP::Flags::ECHO_REQUEST)
						flags_val.SetString("echo_request");
					else if(flags == Tins::ICMP::Flags::INFO_REPLY)
						flags_val.SetString("info_reply");
					else if(flags == Tins::ICMP::Flags::INFO_REQUEST)
						flags_val.SetString("echo_request");
					else if(flags == Tins::ICMP::Flags::PARAM_PROBLEM)
						flags_val.SetString("param_problem");
					else if(flags == Tins::ICMP::Flags::REDIRECT)
						flags_val.SetString("redirect");
					else if(flags == Tins::ICMP::Flags::SOURCE_QUENCH)
						flags_val.SetString("source_quench");
					else if(flags == Tins::ICMP::Flags::TIME_EXCEEDED)
						flags_val.SetString("time_exceeded");
					else if(flags == Tins::ICMP::Flags::TIMESTAMP_REPLY)
						flags_val.SetString("timestamp_reply");
					else if(flags == Tins::ICMP::Flags::TIMESTAMP_REQUEST)
						flags_val.SetString("timestamp_request");
					else
						flags_val.SetString("");
					icmp.AddMember("flags", flags_val, allocator);
					
					int code = p.pdu()->rfind_pdu<Tins::ICMP>().code();
					rapidjson::Value code_val;
					code_val.SetInt(code);
					icmp.AddMember("code", code_val, allocator);
					
					int sequence = p.pdu()->rfind_pdu<Tins::ICMP>().sequence();
					rapidjson::Value sequence_val;
					sequence_val.SetInt(sequence);
					icmp.AddMember("sequence", sequence_val, allocator);
					
					d.AddMember("icmp", icmp, allocator);
				}
				else
				{ }
			}
			int size = p.pdu()->size();
			rapidjson::Value size_val;
			size_val.SetInt(size);
			d.AddMember("size", size_val, allocator);
			
			// TIMESTAMP
			std::string datetime = getCurrentDateTime();
			rapidjson::Value datetime_val;
			datetime_val.SetString(datetime.c_str(), datetime.length(), allocator);
			d.AddMember("datetime", datetime_val, allocator);
		}

		if(p.pdu()->find_pdu<Tins::ARP>())
		{
			rapidjson::Value arp(rapidjson::kObjectType);

			std::string sender_hw_addr = p.pdu()->rfind_pdu<Tins::ARP>().sender_hw_addr().to_string();
			rapidjson::Value sender_hw_addr_val;
			sender_hw_addr_val.SetString(sender_hw_addr.c_str(), sender_hw_addr.length(), allocator);
			arp.AddMember("sender_hw_addr", sender_hw_addr_val, allocator);

			std::string target_hw_addr = p.pdu()->rfind_pdu<Tins::ARP>().target_hw_addr().to_string();
			rapidjson::Value target_hw_addr_val;
			target_hw_addr_val.SetString(target_hw_addr.c_str(), target_hw_addr.length(), allocator);
			arp.AddMember("target_hw_addr", target_hw_addr_val, allocator);

			std::string sender_ip_addr = p.pdu()->rfind_pdu<Tins::ARP>().sender_ip_addr().to_string();
			rapidjson::Value sender_ip_addr_val;
			sender_ip_addr_val.SetString(sender_ip_addr.c_str(), sender_ip_addr.length(), allocator);
			arp.AddMember("sender_ip_addr", sender_ip_addr_val, allocator);

			std::string target_ip_addr = p.pdu()->rfind_pdu<Tins::ARP>().target_ip_addr().to_string();
			rapidjson::Value target_ip_addr_val;
			target_ip_addr_val.SetString(target_ip_addr.c_str(), target_ip_addr.length(), allocator);
			arp.AddMember("target_ip_addr", target_ip_addr_val, allocator);

			d.AddMember("arp", arp, allocator);

			std::string datetime = getCurrentDateTime();
			rapidjson::Value datetime_val;
			datetime_val.SetString(datetime.c_str(), datetime.length(), allocator);
			d.AddMember("datetime", datetime_val, allocator);
		}
		
		// DEBUG
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer, rapidjson::Document::EncodingType, rapidjson::UTF8<>> writer(buffer);
		d.Accept(writer);
		//std::cout << buffer.GetString() << std::endl;
		
		bb.Add(BulkOperation::Index, "{ \"_index\" : \"sniffer\", \"_type\" : \"frame\"}", buffer.GetString());
	}
	std::cout << es.bulk(bb.Get()).GetRawData();
	vt.clear();
}

bool doo(Tins::Packet& packet)
{
	vt.push_back(packet);
	if(vt.size() == 200)
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

    return 0;
}
