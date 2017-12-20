#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <unistd.h>

#include "cjson.h"

#define ARP_REQUEST 1
#define ARP_REPLY 2

struct arphdr_t{ 
    uint16_t htype;
    uint16_t ptype;
    u_char hlen;
    u_char plen;
    uint16_t oper;
    u_char sha[6];
    u_char spa[4];
    u_char tha[6];
    u_char tpa[4]; 
}; 




static int print_preallocated(cJSON *root)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    size_t len = 0;
    size_t len_fail = 0;

    /* formatted print */
    out = cJSON_Print(root);

    /* create buffer to succeed */
    /* the extra 5 bytes are because of inaccuracies when reserving memory */
    len = strlen(out) + 5;
    buf = (char*)malloc(len);
    if (buf == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* create buffer to fail */
    len_fail = strlen(out);
    buf_fail = (char*)malloc(len_fail);
    if (buf_fail == NULL)
    {
        printf("Failed to allocate memory.\n");
        exit(1);
    }

    /* Print to buffer */
    if (!cJSON_PrintPreallocated(root, buf, (int)len, 1)) {
        printf("cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            printf("cJSON_Print result:\n%s\n", out);
            printf("cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    /* success */
    printf("%s\n", buf);

    /* force it to fail */
    if (cJSON_PrintPreallocated(root, buf_fail, (int)len_fail, 1)) {
        printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        printf("cJSON_Print result:\n%s\n", out);
        printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}










int main(){
	int sock_r;
	struct sockaddr saddr;
	int saddr_len;
	int buflen;
	struct sockaddr_in source,dest;
	
	sock_r = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock_r < 0)
	{
		printf("error in socket()\n");
		return -1;
	}
	
	unsigned char *buffer = (unsigned char *) malloc(65536);
	
	while(1){
		memset(buffer, 0, 65536);
		saddr_len = sizeof(saddr);
		
		buflen = recvfrom(sock_r, buffer, 65536, 0, &saddr, (socklen_t *) &saddr_len);
		if(buflen < 0)
		{
			printf("error in recvfrom");
			return -1;
		}
		cJSON *root = cJSON_CreateObject();
		char source_addr[18];
		char destination_addr[18];
		struct ethhdr *eth = (struct ethhdr *)(buffer);
		printf("\nEthernet Header\n");
		printf("\t|-Source Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
		snprintf(source_addr, 18, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", eth->h_source[0],eth->h_source[1],eth->h_source[2],eth->h_source[3],eth->h_source[4],eth->h_source[5]);
		printf("\t|-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X\n",eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
		snprintf(destination_addr, 18, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", eth->h_dest[0],eth->h_dest[1],eth->h_dest[2],eth->h_dest[3],eth->h_dest[4],eth->h_dest[5]);
		
		cJSON *fmt = NULL;
		cJSON_AddItemToObject(root, "eth", fmt = cJSON_CreateObject());
		cJSON_AddItemToObject(fmt, "src", cJSON_CreateString(source_addr));
		cJSON_AddItemToObject(fmt, "dst", cJSON_CreateString(destination_addr));
		
		if(eth->h_proto == 0x0608)
		{
			struct arphdr_t *arph = (struct arphdr_t *)(buffer + sizeof(struct ethhdr));
			printf("\nARP Header\n");
			printf("\t|-Hardware type: %s\n", (ntohs(arph->htype) == 1) ? "Ethernet" : "Unknown"); 
			printf("\t|-Protocol type: %s\n", (ntohs(arph->ptype) == 0x0800) ? "IPv4" : "Unknown"); 
			printf("\t|-Operation: %s\n", (ntohs(arph->oper) == ARP_REQUEST)? "ARP Request" : "ARP Reply"); 
			if (ntohs(arph->htype) == 1 && ntohs(arph->ptype) == 0x0800){ 
				int i;
				printf("Sender MAC: "); 
				for(i=0; i<6;i++)
					printf("%02X:", arph->sha[i]); 

				printf("\nSender IP: "); 
				for(i=0; i<4;i++)
					printf("%d.", arph->spa[i]); 

				printf("\nTarget MAC: "); 
				for(i=0; i<6;i++)
					printf("%02X:", arph->tha[i]); 

				printf("\nTarget IP: "); 
				for(i=0; i<4; i++)
					printf("%d.", arph->tpa[i]); 
				printf("\n"); 

			} 
		}
		else if(eth->h_proto == 0x0008)
		{
			struct iphdr *iph = (struct iphdr*) (buffer + sizeof(struct ethhdr));
			unsigned short iphdrlen;
			 
			iphdrlen =iph->ihl*4;
			 
			memset(&source, 0, sizeof(source));
			source.sin_addr.s_addr = iph->saddr;
			 
			memset(&dest, 0, sizeof(dest));
			dest.sin_addr.s_addr = iph->daddr;
			 
			fprintf(stdout,"\n");
			fprintf(stdout,"IP Header\n");
			fprintf(stdout,"   |-IP Version        : %d\n",(unsigned int)iph->version);
			fprintf(stdout,"   |-IP Header Length  : %d DWORDS or %d Bytes\n",(unsigned int)iph->ihl,((unsigned int)(iph->ihl))*4);
			fprintf(stdout,"   |-Type Of Service   : %d\n",(unsigned int)iph->tos);
			fprintf(stdout,"   |-IP Total Length   : %d  Bytes(Size of Packet)\n",ntohs(iph->tot_len));
			fprintf(stdout,"   |-Identification    : %d\n",ntohs(iph->id));
			//fprintf(logfile,"   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
			//fprintf(logfile,"   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
			//fprintf(logfile,"   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
			fprintf(stdout,"   |-TTL      : %d\n",(unsigned int)iph->ttl);
			fprintf(stdout,"   |-Protocol : %d\n",(unsigned int)iph->protocol);
			fprintf(stdout,"   |-Checksum : %d\n",ntohs(iph->check));
			fprintf(stdout,"   |-Source IP        : %s\n",inet_ntoa(source.sin_addr));
			fprintf(stdout,"   |-Destination IP   : %s\n",inet_ntoa(dest.sin_addr));
			
			if(iph->protocol == 1)
			{
			    struct icmphdr *icmph = (struct icmphdr *)(buffer + iphdrlen + sizeof(struct ethhdr));
				fprintf(stdout,"\nICMP Header\n");
				fprintf(stdout,"   |-Type : %d",(unsigned int)(icmph->type));
						 
				if((unsigned int)(icmph->type) == 11) 
					fprintf(stdout,"  (TTL Expired)\n");
				else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY) 
					fprintf(stdout,"  (ICMP Echo Reply)\n");
				fprintf(stdout,"   |-Code : %d\n",(unsigned int)(icmph->code));
				fprintf(stdout,"   |-Checksum : %d\n",ntohs(icmph->checksum));
				//fprintf(stdout,"   |-ID       : %d\n",ntohs(icmph->id));
				//fprintf(stdout,"   |-Sequence : %d\n",ntohs(icmph->sequence));
				fprintf(stdout,"\n");
			}
			else if(iph->protocol == 6)
			{	 
				struct tcphdr *tcph = (struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
					 
				fprintf(stdout,"\n");
				fprintf(stdout,"TCP Header\n");
				fprintf(stdout,"   |-Source Port      : %u\n",ntohs(tcph->source));
				fprintf(stdout,"   |-Destination Port : %u\n",ntohs(tcph->dest));
				fprintf(stdout,"   |-Sequence Number    : %u\n",ntohl(tcph->seq));
				fprintf(stdout,"   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
				fprintf(stdout,"   |-Header Length      : %d DWORDS or %d BYTES\n" ,(unsigned int)tcph->doff,(unsigned int)tcph->doff*4);
				//fprintf(stdout,"   |-CWR Flag : %d\n",(unsigned int)tcph->cwr);
				//fprintf(stdout,"   |-ECN Flag : %d\n",(unsigned int)tcph->ece);
				fprintf(stdout,"   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
				fprintf(stdout,"   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
				fprintf(stdout,"   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
				fprintf(stdout,"   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
				fprintf(stdout,"   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
				fprintf(stdout,"   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
				fprintf(stdout,"   |-Window         : %d\n",ntohs(tcph->window));
				fprintf(stdout,"   |-Checksum       : %d\n",ntohs(tcph->check));
				fprintf(stdout,"   |-Urgent Pointer : %d\n",tcph->urg_ptr);
				fprintf(stdout,"\n");
			}
			else if(iph->protocol == 17)
			{
				struct udphdr *udph = (struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));
				 
				fprintf(stdout,"\nUDP Header\n");
				fprintf(stdout,"   |-Source Port      : %d\n" , ntohs(udph->source));
				fprintf(stdout,"   |-Destination Port : %d\n" , ntohs(udph->dest));
				fprintf(stdout,"   |-UDP Length       : %d\n" , ntohs(udph->len));
				fprintf(stdout,"   |-UDP Checksum     : %d\n" , ntohs(udph->check));
			}
			
		}
		printf("###############################################\n");
		if (print_preallocated(root) != 0) {
			cJSON_Delete(fmt);
			cJSON_Delete(root);
			exit(EXIT_FAILURE);
		}
		cJSON_Delete(root);
	}
}
