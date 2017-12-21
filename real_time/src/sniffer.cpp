#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <mutex>
#include <thread>
#include <typeinfo>
#include <vector>

#include "../include/elasticsearch/elasticsearch.h"
#include "utils.h"
#include "arphdr.h"

#define BUFSIZE 65536

std::mutex m;
Elasticsearch es{};
BulkBody bb;

std::string save()
{
    return es.bulk(bb.Get()).GetRawData();
}

void processFrame(unsigned char *buffer)
{
    struct ethhdr *eth = (struct ethhdr *)(buffer);
    char source_addr[18];
    char destination_addr[18];
    snprintf(source_addr, 18, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
    snprintf(destination_addr, 18, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    std::string source = std::string(source_addr);
    std::string destination = std::string(destination_addr);

    if(source == "00:00:00:00:00:00" && destination == "00:00:00:00:00:00")
        return;

    rapidjson::Document d;
    d.SetObject();
    rapidjson::Document::AllocatorType& allocator = d.GetAllocator();

    rapidjson::Value ethernet(rapidjson::kObjectType);
    
    rapidjson::Value src_addr_val;
    src_addr_val.SetString(source.c_str(), source.length(), allocator);
    ethernet.AddMember("src", src_addr_val, allocator);

    rapidjson::Value dest_addr_val;
    dest_addr_val.SetString(destination.c_str(), destination.length(), allocator);
    ethernet.AddMember("dest", dest_addr_val, allocator);

    d.AddMember("ethernet", ethernet, allocator);

    if(eth->h_proto == 0x0608)
    {
        struct arphdr_t *arph = (struct arphdr_t *)(buffer + sizeof(struct ethhdr));
        if(ntohs(arph->htype) == 1 && ntohs(arph->ptype) == 0x0800)
        {
            rapidjson::Value arp(rapidjson::kObjectType);

            char sender_mac[18];
            char target_mac[18];
            char sender_ip[16];
            char target_ip[16];
            snprintf(sender_mac, 18, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", arph->sha[0], arph->sha[1], arph->sha[2], arph->sha[3], arph->sha[4], arph->sha[5]);
            snprintf(target_mac, 18, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", arph->tha[0], arph->tha[1], arph->tha[2], arph->tha[3], arph->tha[4], arph->tha[5]);
            snprintf(sender_ip, 16, "%d.%d.%d.%d", arph->spa[0], arph->spa[1], arph->spa[2], arph->spa[3]);
            snprintf(target_ip, 16, "%d.%d.%d.%d", arph->tpa[0], arph->tpa[1], arph->tpa[2], arph->tpa[3]);

            rapidjson::Value sender_hw_val;
            sender_hw_val.SetString(std::string(sender_mac).c_str(), std::string(sender_mac).length(), allocator);
            arp.AddMember("sender_hw", sender_hw_val, allocator);
            
            rapidjson::Value target_hw_val;
            target_hw_val.SetString(std::string(target_mac).c_str(), std::string(target_mac).length(), allocator);
            arp.AddMember("target_hw", target_hw_val, allocator);

            rapidjson::Value sender_ip_val;
            sender_ip_val.SetString(std::string(sender_ip).c_str(), std::string(sender_ip).length(), allocator);
            arp.AddMember("sender_ip", sender_ip_val, allocator);

            rapidjson::Value target_ip_val;
            target_ip_val.SetString(std::string(target_ip).c_str(), std::string(target_ip).length(), allocator);
            arp.AddMember("target_ip", target_ip_val, allocator);

            rapidjson::Value hw_type_val;
            hw_type_val.SetInt(ntohs(arph->htype));
            arp.AddMember("hw_type", hw_type_val, allocator);

            rapidjson::Value proto_type_val;
            proto_type_val.SetInt(ntohs(arph->ptype));
            arp.AddMember("proto_type", proto_type_val, allocator);

            rapidjson::Value operation_val;
            operation_val.SetInt(ntohs(arph->oper));
            arp.AddMember("operation", operation_val, allocator);

            d.AddMember("arp", arp, allocator);
        }
    }
    else if(eth->h_proto == 0x0008)
    {
        unsigned short iphdrlen;
        struct sockaddr_in source_addr;
        struct sockaddr_in destination_addr;

        struct iphdr *iph = (struct iphdr*) (buffer + sizeof(struct ethhdr));
        iphdrlen =iph->ihl*4;
            
        memset(&source_addr, 0, sizeof(source_addr));
        source_addr.sin_addr.s_addr = iph->saddr;
            
        memset(&destination_addr, 0, sizeof(destination_addr));
        destination_addr.sin_addr.s_addr = iph->daddr;

        rapidjson::Value ip(rapidjson::kObjectType);

        rapidjson::Value version_val;
        version_val.SetUint((unsigned int) iph->version);
        ip.AddMember("version", version_val, allocator);

        rapidjson::Value ttl_val;
        ttl_val.SetUint((unsigned int) iph->ttl);
        ip.AddMember("ttl", ttl_val, allocator);

        rapidjson::Value proto_val;
        proto_val.SetUint((unsigned int) iph->protocol);
        ip.AddMember("protocol", proto_val, allocator);

        std::string source_ip = std::string(inet_ntoa(source_addr.sin_addr));
        rapidjson::Value source_ip_val;
        source_ip_val.SetString(source_ip.c_str(), source_ip.length(), allocator);
        ip.AddMember("src", source_ip_val, allocator);
    
        std::string destination_ip = std::string(inet_ntoa(destination_addr.sin_addr));
        rapidjson::Value destination_ip_val;
        destination_ip_val.SetString(destination_ip.c_str(), destination_ip.length(), allocator);
        ip.AddMember("dest", destination_ip_val, allocator);

        d.AddMember("ip", ip, allocator);

        if(iph->protocol == 1)
        {
            struct icmphdr *icmph = (struct icmphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));

            rapidjson::Value icmp(rapidjson::kObjectType);

            rapidjson::Value type_val;
            type_val.SetUint((unsigned int) icmph->type);
            icmp.AddMember("type", type_val, allocator);

            d.AddMember("icmp", icmp, allocator);
        }
        else if(iph->protocol == 6)
        {
            struct tcphdr *tcph = (struct tcphdr*) (buffer + iphdrlen + sizeof(struct ethhdr));

            rapidjson::Value tcp(rapidjson::kObjectType);

            rapidjson::Value src_port_val;
            src_port_val.SetUint(ntohs(tcph->source));
            tcp.AddMember("src_port", src_port_val, allocator);

            rapidjson::Value dest_port_val;
            dest_port_val.SetUint(ntohs(tcph->dest));
            tcp.AddMember("dest_port", dest_port_val, allocator);

            rapidjson::Value seq_val;
            seq_val.SetUint(ntohs(tcph->seq));
            tcp.AddMember("seq", seq_val, allocator);

            rapidjson::Value ack_seq_val;
            ack_seq_val.SetUint(ntohs(tcph->ack_seq));
            tcp.AddMember("ack_seq", ack_seq_val, allocator);

            rapidjson::Value urg_flag_val;
            urg_flag_val.SetBool((tcph->urg != 0));
            tcp.AddMember("urg", urg_flag_val, allocator);

            rapidjson::Value ack_flag_val;
            ack_flag_val.SetBool((tcph->ack != 0));
            tcp.AddMember("ack", ack_flag_val, allocator);

            rapidjson::Value psh_flag_val;
            psh_flag_val.SetBool((tcph->psh != 0));
            tcp.AddMember("psh", psh_flag_val, allocator);

            rapidjson::Value rst_flag_val;
            rst_flag_val.SetBool((tcph->rst != 0));
            tcp.AddMember("rst", rst_flag_val, allocator);            

            rapidjson::Value syn_flag_val;
            syn_flag_val.SetBool((tcph->syn != 0));
            tcp.AddMember("syn", syn_flag_val, allocator);  

            rapidjson::Value fin_flag_val;
            fin_flag_val.SetBool((tcph->fin != 0));
            tcp.AddMember("fin", fin_flag_val, allocator);  

            d.AddMember("tcp", tcp, allocator);
        }
        else if(iph->protocol == 17)
        {
            struct udphdr *udph = (struct udphdr*) (buffer + iphdrlen + sizeof(struct ethhdr));

            rapidjson::Value udp(rapidjson::kObjectType);

            rapidjson::Value src_port_val;
            src_port_val.SetUint(ntohs(udph->source));
            udp.AddMember("src_port", src_port_val, allocator);

            rapidjson::Value dest_port_val;
            dest_port_val.SetUint(ntohs(udph->dest));
            udp.AddMember("dest_port", dest_port_val, allocator);

            d.AddMember("udp", udp, allocator);
        }
    }
    std::string datetime = getCurrentDateTime();
	rapidjson::Value datetime_val;
	datetime_val.SetString(datetime.c_str(), datetime.length(), allocator);
	d.AddMember("datetime", datetime_val, allocator);

    rapidjson::StringBuffer strbuffer;
	rapidjson::Writer<rapidjson::StringBuffer, rapidjson::Document::EncodingType, rapidjson::UTF8<>> writer(strbuffer);
	d.Accept(writer);

    bb.Add(BulkOperation::Index, "{ \"_index\" : \"sniffer\", \"_type\" : \"frame\"}", strbuffer.GetString());
}

int main(int argc, char *argv[])
{
    int sock_r;
    int buflen;

    sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock_r < 0)
    {
        printf("error in socket()\n");
        return -1;
    }

    // struct ifreq ifr;
    // memset(&ifr, 0, sizeof(ifr));
    // snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", argv[1]);
    // if(ioctl(sock_r, SIOCGIFINDEX, &ifr) < 0)
    // {
    //     perror("ioctl() failed to find interface");
    //     return -1;
    // }
    // if (setsockopt(sock_r, SOL_SOCKET, SO_BINDTODEVICE, (void *)&ifr, sizeof(ifr)) < 0) {
    //     perror("Unable to bind to device.");
    //     return -1;
    // }

    unsigned char *buffer = (unsigned char *) malloc(BUFSIZE);
    while(true)
    {
        memset(buffer, 0, BUFSIZE);

        buflen = recvfrom(sock_r, buffer, BUFSIZE, 0, NULL, NULL);
        if(buflen < 0)
        {
            fprintf(stderr, "error in recvfrom()\n");
            continue;
        }

        processFrame(buffer);

        if(bb.Count() > 20)
        {
            std::cerr << save() << std::endl;
            bb.Clear();
        }
    }
    free(buffer);
    return 0;
}