#ifndef ARPHDR_H_
#define ARPHDR_H_

#include <inttypes.h>
#include <stdlib.h>

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

#endif