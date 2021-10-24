#include "common.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <cstdint>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

namespace traceroute
{
namespace
{
    
const IcmpHeader *toIcmpHeaderT(const char *p)
{
    return reinterpret_cast<const IcmpHeader *>(p);
}
const TcpHeader *toTcpHeaderT(const char *p)
{
    return reinterpret_cast<const TcpHeader *>(p);
}
} // namespace

struct IpHeaderFirstByte
{
    uint8_t pad : 4;
    uint8_t version : 4;
};

size_t getIpHeaderSize(const char *ipHdr)
{
    const IpHeaderFirstByte *ipHeaderFb = reinterpret_cast<const IpHeaderFirstByte *>(ipHdr);

    if (ipHeaderFb->version == 4)
    {
        const Ipv4Header *ip4header = reinterpret_cast<const Ipv4Header *>(ipHdr);
        assert(ip4header->version == 4);
        auto iphdr_size = ip4header->ihl << 2;
        return iphdr_size;
    }
    return sizeof(Ipv6Header);
}

bool validateSameSequence(const char *tcpReply, const TcpHeader &tcpEcho)
{
    return toTcpHeaderT(tcpReply)->seq == tcpEcho.seq;
}

bool validateSession(const char *icmpReply, const IcmpHeader &icmpEcho)
{
    return toIcmpHeaderT(icmpReply)->id == icmpEcho.id;
}

int getIcmpType(const char *icmpHeader)
{
    return toIcmpHeaderT(icmpHeader)->type;
}
} // namespace traceroute