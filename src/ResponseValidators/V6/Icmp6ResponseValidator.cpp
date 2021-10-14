#include "Icmp6ResponseValidator.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <netinet/icmp6.h>

namespace traceroute::responseValidators::v6
{

using namespace traceroute::packet;
bool Icmp6ResponseValidator::validate(const Packet &request, const SocketAddress &client, int protocol,
                                      const char *response, size_t responseSize)
{
    bool isResponseValid = false;
    const auto &icmpPacket = dynamic_cast<const IcmpPacket &>(request);

    const IcmpHeader *header = reinterpret_cast<const IcmpHeader *>(response);
    if (header->type == ICMP6_TIME_EXCEEDED)
    {
        response += sizeof(IcmpHeader);
        response += Ipv6HeaderSize;
        const IcmpHeader *inner_icmp_header = reinterpret_cast<const IcmpHeader *>(response);
        if (inner_icmp_header->id == icmpPacket.GetIcmpHeader().id)
        {
            isResponseValid = true;
        }
    }
    else if ((header->type == ICMP6_ECHO_REPLY) && header->id == icmpPacket.GetIcmpHeader().id &&
             icmpPacket.getDestinationAddress() == client)
    {
        isResponseValid = true;
    }
    return isResponseValid;
}
} // namespace traceroute::responseValidators::v6
