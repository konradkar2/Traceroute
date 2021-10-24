#pragma once
#include <cstdint>

using namespace std;
namespace traceroute
{
#define Ipv6HeaderSize 40;

struct Ipv6Header
{
    uint8_t pad : 4;
    uint8_t version : 4;
    uint8_t content[39];
};

struct Ipv4Header
{
    uint8_t ihl : 4;
    uint8_t version : 4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
};

struct IcmpHeader
{
    uint8_t type; /* message type */
    uint8_t code; /* type sub-code */
    uint16_t checksum;
    uint16_t id;
    uint16_t sequence;
};
struct TcpHeader
{
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;
    uint8_t th_x2 : 4;  /* (unused) */
    uint8_t th_off : 4; /* data offset */
    uint8_t th_flags;
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
};
struct UdpHeader
{
    uint16_t source;
    uint16_t dest;
    uint16_t len;
    uint16_t check;
};
} // namespace traceroute