#include "Icmp4ResponseValidator.hpp"
#include "Utils.hpp"
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include "ResponseValidators/common.hpp"

namespace traceroute::responseValidators::v4
{
using namespace traceroute::packet;
namespace
{
bool validateEchoReply(const char *icmpHeader, const IcmpPacket &icmpPacket, const SocketAddress &client)
{
    if (client == icmpPacket.getDestinationAddress())
        return validateSession(icmpHeader, icmpPacket.GetIcmpHeader());
    return false;
}
bool validateTimeExceeded(const char *icmpHeader, const IcmpPacket &icmpPacket)
{
    icmpHeader += sizeof(IcmpHeader);
    icmpHeader += getIpHeaderSize(icmpHeader);
    return validateSession(icmpHeader, icmpPacket.GetIcmpHeader());
}


} // namespace

bool Icmp4ResponseValidator::validateFields(const Packet &request, const SocketAddress &client, const char *response,
                                            size_t responseSize)
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

bool Icmp4ResponseValidator::validateSize(size_t size)
{
    return true;
}

bool Icmp4ResponseValidator::validateProtocol(int protocol)
{
    return protocol == IPPROTO_ICMP;
}



} // namespace traceroute::responseValidators::v4
