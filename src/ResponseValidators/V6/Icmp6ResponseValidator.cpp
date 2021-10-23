#include "Icmp6ResponseValidator.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include "ResponseValidators/common.hpp"
namespace traceroute::responseValidators::v6
{

using namespace traceroute::packet;
bool validateEchoReply(const char *icmpHeader, const IcmpPacket &icmpPacket, const SocketAddress &client)
{
    if (client == icmpPacket.getDestinationAddress())
        return validateSession(icmpHeader, icmpPacket.GetIcmpHeader());
    return false;
}
bool validateTimeExceeded(const char *icmpHeader, const IcmpPacket &icmpPacket)
{
    icmpHeader += sizeof(IcmpHeader);
    icmpHeader += Ipv6HeaderSize;
    return validateSession(icmpHeader, icmpPacket.GetIcmpHeader());
}
bool Icmp6ResponseValidator::validateFields(const Packet &request, const SocketAddress &client, const char *response,
                                            size_t responseSize)
{
    const auto &icmpPacket = dynamic_cast<const IcmpPacket &>(request);

    switch(getIcmpType(response))
    {
    case(ICMP6_ECHO_REPLY):
        return validateEchoReply(response,icmpPacket,client);
    case(ICMP6_TIME_EXCEEDED):
        return validateTimeExceeded(response, icmpPacket);
    default:
        return false;
    }
}

bool Icmp6ResponseValidator::validateSize(size_t size)
{
    return true;
}

bool Icmp6ResponseValidator::validateProtocol(int protocol)
{
    return protocol == IPPROTO_ICMPV6;
}

} // namespace traceroute::responseValidators::v6
