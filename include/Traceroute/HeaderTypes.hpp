#pragma once
#include <cstdint>

using namespace std;
namespace traceroute
{

struct Ipv6Header
{
    std::uint8_t pad : 4;
    std::uint8_t version : 4;
    std::uint8_t content[39];
};

struct Ipv4Header
{
    std::uint8_t ihl : 4;
    std::uint8_t version : 4;
    std::uint8_t tos;
    std::uint16_t tot_len;
    std::uint16_t id;
    std::uint16_t frag_off;
    std::uint8_t ttl;
    std::uint8_t protocol;
    std::uint16_t check;
    std::uint32_t saddr;
    std::uint32_t daddr;
};

struct IcmpHeader
{
    std::uint8_t type; /* message type */
    std::uint8_t code; /* type sub-code */
    std::uint16_t checksum;
    std::uint16_t id;
    std::uint16_t sequence;
};
struct TcpHeader
{
    std::uint16_t source;
    std::uint16_t dest;
    std::uint32_t seq;
    std::uint32_t ack_seq;
    std::uint8_t th_x2 : 4;  /* (unused) */
    std::uint8_t th_off : 4; /* data offset */
    std::uint8_t th_flags;
    std::uint16_t window;
    std::uint16_t check;
    std::uint16_t urg_ptr;
};
struct UdpHeader
{
    std::uint16_t source;
    std::uint16_t dest;
    std::uint16_t len;
    std::uint16_t check;
};
} // namespace traceroute