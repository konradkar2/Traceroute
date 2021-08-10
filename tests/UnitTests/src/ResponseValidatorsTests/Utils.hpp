#pragma once

#include <string>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>

namespace ResponseValidatorsTests
{
    template <typename RequestProtocol>
    struct ResponseIcmp
    {
        struct ReceivedPacketCopy
        {
            Traceroute::Ipv4Header ipv4Header;
            RequestProtocol protocolHeader;
        };
        Traceroute::Ipv4Header ipv4Header;
        Traceroute::IcmpHeader icmpHeader;
        ReceivedPacketCopy receivedPacketCopy;
    };

    void fillIPv4Header(const Traceroute::SocketAddress &source, const Traceroute::SocketAddress &destination, int ihl,
                        int protocol, Traceroute::Ipv4Header *ipHeader);
}
