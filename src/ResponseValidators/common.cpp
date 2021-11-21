#include "common.hpp"
#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <cstdint>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <stdexcept>

namespace traceroute {
namespace {

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
        fprintf(stderr,"Its version 4\n");
        const Ipv4Header *ip4header  = reinterpret_cast<const Ipv4Header *>(ipHdr);
        auto              iphdr_size = ip4header->ihl << 2;
        fprintf(stderr,"Ip header size: %d \n",iphdr_size);
        return iphdr_size;
    }
    if (ipHeaderFb->version == 6)
    {
        return sizeof(Ipv6Header);
    }
    throw std::runtime_error("Response does not contain valid ipHeader");
}
bool isIpHeaderPresent(const ResponseInfo &responseInfo, const char *response)
{
    if (responseInfo.size() < sizeof(IpHeaderFirstByte))
    {
        throw std::runtime_error("Response does not contain an ipHeader");
    }
    const IpHeaderFirstByte *ipHeaderFb = reinterpret_cast<const IpHeaderFirstByte *>(response);

    return ipHeaderFb->version == 4 || ipHeaderFb->version == 6;
}

std::pair<ResponseInfo, const char *> skipIpHeader(const ResponseInfo &responseInfo, const char *response)
{
    auto headerSize      = getIpHeaderSize(response);
    auto newResponseSize = responseInfo.size() - headerSize;
    response += headerSize;

    return {ResponseInfo(responseInfo.client(), responseInfo.protocol(), newResponseSize), response};
}

bool validateSameSequence(const char *tcpReply, const TcpHeader &tcpEcho)
{
    return toTcpHeaderT(tcpReply)->seq == tcpEcho.seq;
}

bool validateSession(const char *icmpReply, const IcmpHeader &icmpEcho)
{
    fprintf(stderr,"reply id: %d, request id: %d\n", (int)toIcmpHeaderT(icmpReply)->id,(int)icmpEcho.id);
    return toIcmpHeaderT(icmpReply)->id == icmpEcho.id;
}

int getIcmpType(const char *icmpHeader)
{
    return toIcmpHeaderT(icmpHeader)->type;
}
} // namespace traceroute