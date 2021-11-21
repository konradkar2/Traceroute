#include "ResponseValidators/common.hpp"
#include "common.hpp"
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include <cassert>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

namespace traceroute::responseValidators {
using namespace traceroute::packet;
namespace {
bool validateEchoReply(const char *icmpHeader, const IcmpPacket &icmpPacket, const SocketAddress &client)
{
    if (client == icmpPacket.getDestinationAddress())
        return validateSession(icmpHeader, icmpPacket.GetIcmpHeader());
    return false;
}
bool validateTimeExceeded(const char *icmpHeader, const IcmpPacket &icmpPacket)
{
    const char *ipHeader        = icmpHeader + sizeof(IcmpHeader);
    const char *innerIcmpHeader = ipHeader + getIpHeaderSize(ipHeader);
    return validateSession(innerIcmpHeader, icmpPacket.GetIcmpHeader());
}

} // namespace
IcmpResponseValidator::IcmpResponseValidator(const packet::IcmpPacket &icmpPacket) : mIcmpPacket(icmpPacket)
{
}

bool IcmpResponseValidator::validateFields(const ResponseInfo &responseInfo, const char *response)
{
    switch (getIcmpType(response))
    {
    case ICMP_ECHOREPLY:
    case ICMP6_ECHO_REPLY:
        return validateEchoReply(response, mIcmpPacket, responseInfo.client());
    case ICMP_TIME_EXCEEDED:
    case ICMP6_TIME_EXCEEDED:
        return validateTimeExceeded(response, mIcmpPacket);
    default:
        return false;
    }
}

bool IcmpResponseValidator::validateSize(size_t size)
{
    return true;
}

bool IcmpResponseValidator::validateProtocol(int protocol)
{
    return protocol == IPPROTO_ICMP || protocol == IPPROTO_ICMPV6;
}

} // namespace traceroute::responseValidators
