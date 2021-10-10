#include "Icmp4ResponseValidator.hpp"
#include "Utils.hpp"
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <netinet/ip_icmp.h>
namespace traceroute::responseValidators::v4
{
using namespace traceroute::packet;
namespace
{
bool validateEchoReply(const char *icmpHeader, const IcmpPacket &icmpPacket, const SocketAddress &client);
bool validateTimeExceeded(const char *icmpHeader, const IcmpPacket &icmpPacket);
bool validateIcmpId(const char *icmpHeader, const IcmpPacket &icmpPacket);
int getIcmpType(const char *icmpHeader);
} // namespace

bool Icmp4ResponseValidator::validate(const Packet &request, const SocketAddress &client, int protocol,
                                      const char *response, size_t responseSize)
{
    const auto &icmpPacket = dynamic_cast<const IcmpPacket &>(request);
    response += getIpHeaderSize(response);
    switch (getIcmpType(response))
    {
    case ICMP_ECHOREPLY:
        return validateEchoReply(response, icmpPacket, client);
    case ICMP_TIME_EXCEEDED:
        return validateTimeExceeded(response, icmpPacket);
    default:
        return false;
    }
}

namespace
{
bool validateEchoReply(const char *icmpHeader, const IcmpPacket &icmpPacket, const SocketAddress &client)
{
    if (client == icmpPacket.getDestinationAddress())
        return validateIcmpId(icmpHeader, icmpPacket);
    return false;
}
bool validateTimeExceeded(const char *icmpHeader, const IcmpPacket &icmpPacket)
{
    icmpHeader += sizeof(IcmpHeader);
    icmpHeader += getIpHeaderSize(icmpHeader);
    return validateIcmpId(icmpHeader, icmpPacket);
}
bool validateIcmpId(const char *icmpHeader, const IcmpPacket &icmpPacket)
{
    const IcmpHeader *icmpH = reinterpret_cast<const IcmpHeader *>(icmpHeader);
    if (icmpH->id == icmpPacket.GetIcmpHeader().id)
        return true;
    return false;
}
int getIcmpType(const char *icmpHeader)
{
    const IcmpHeader *icmpHdr = reinterpret_cast<const IcmpHeader *>(icmpHeader);
    return icmpHdr->type;
}

} // namespace

} // namespace traceroute::responseValidators::v4
