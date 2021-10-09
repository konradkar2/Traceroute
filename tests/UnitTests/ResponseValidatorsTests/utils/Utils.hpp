#pragma once

#include <string>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>

namespace traceroute
{
    template <typename RequestProtocol>
    struct ReceivedPacketCopy
    {
        Ipv4Header ipv4Header;
        RequestProtocol protocolHeader;
    };
    struct ResponseIcmpToIcmp
    {

        Ipv4Header ipv4Header;
        IcmpHeader icmpHeader;
        ReceivedPacketCopy<traceroute::IcmpHeader> receivedPacketCopy;
    };
    struct ResponseIcmpToTcp
    {

        Ipv4Header ipv4Header;
        IcmpHeader icmpHeader;
        ReceivedPacketCopy<traceroute::TcpHeader> receivedPacketCopy;
    };
    struct ResponseIcmpToUdp
    {

        Ipv4Header ipv4Header;
        IcmpHeader icmpHeader;
        ReceivedPacketCopy<traceroute::UdpHeader> receivedPacketCopy;
    };
    // struct ResponseTcpToTcp
    // {

    //     Ipv4Header ipv4Header;
    //     IcmpHeader icmpHeader;
    //     ReceivedPacketCopy<traceroute::IcmpHeader> receivedPacketCopy;
    // };

    void fillIPv4Header(const SocketAddress &source, const SocketAddress &destination, int ihl,
                        int protocol, Ipv4Header *ipHeader);
}
