#include "Icmp6ResponseValidator.hpp"
#include <netinet/icmp6.h>
#include <cassert>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>

namespace traceroute::responseValidators::v6
{

    using namespace traceroute::packet;
    bool Icmp6ResponseValidator::isResponseValid(const Packet &request, const SocketAddress &client,
                                                 int protocol, const char *response, size_t responseSize)
    {
        bool isResponseValid = false;
        const auto &icmpPacket = dynamic_cast<const IcmpPacket &>(request);
        const char *pResponse = response;

        const IcmpHeader *header = reinterpret_cast<const IcmpHeader *>(pResponse);
        if (header->type == ICMP6_TIME_EXCEEDED)
        {
            pResponse += sizeof(IcmpHeader);
            pResponse += Ipv6HeaderSize;
            const IcmpHeader *inner_icmp_header = reinterpret_cast<const IcmpHeader *>(pResponse);
            if (inner_icmp_header->id == icmpPacket.GetIcmpHeader().id)
            {
                isResponseValid = true;
            }
        }
        else if ((header->type == ICMP6_ECHO_REPLY) &&
                 header->id == icmpPacket.GetIcmpHeader().id &&
                 icmpPacket.getDestinationAddress() == client)
        {
            isResponseValid = true;
        }
        return isResponseValid;
    }
}
