#include "common.hpp"
#include <Traceroute/Packet/UdpPacket.hpp>
#include <Traceroute/ResponseValidators/UdpResponseValidator.hpp>
#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>

namespace traceroute::responseValidators {

bool UdpResponseValidator::validate(const Packet &request, const SocketAddress &client, int protocol,
                                    const char *response, size_t responseSize)
{
    // for now we can't check anything due to UDP header simplicity
    // TODO: send additional data with UDP header in order to validate TIME_EXCEEDED and DST_UNREACH
    const auto &udpPacket = dynamic_cast<const packet::UdpPacket &>(request);
    bool isResponseValid = false;

    if (client.isV4())
    {
        auto ipHeaderSize = getIpHeaderSize(response);
        response += ipHeaderSize;
        responseSize -= ipHeaderSize;
    }

    switch (protocol)
    {
    case IPPROTO_ICMP: {
        const IcmpHeader *header = reinterpret_cast<const IcmpHeader *>(response);
        if (header->type == ICMP_TIME_EXCEEDED)
        {
            isResponseValid = true;
        }
        else if (header->type == ICMP_DEST_UNREACH && udpPacket.getDestinationAddress() == client)
        {
            isResponseValid = true;
        }
        break;
    }
    case IPPROTO_ICMPV6: {
        const IcmpHeader *header = reinterpret_cast<const IcmpHeader *>(response);
        if (header->type == ICMP6_TIME_EXCEEDED)
        {

            isResponseValid = true;
        }
        else if (header->type == ICMP6_DST_UNREACH && udpPacket.getDestinationAddress() == client)
        {
            isResponseValid = true;
        }
        break;
    }
    }

    return isResponseValid;
}

} // namespace traceroute::responseValidators
