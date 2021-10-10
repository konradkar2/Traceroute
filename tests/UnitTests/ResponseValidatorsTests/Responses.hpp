#pragma once

#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>
#include <string>

namespace traceroute
{
template <typename InternetProtocol, typename TransportProtocol>
struct TriggerPacket
{
    InternetProtocol ipHeader;
    TransportProtocol transportHeader;
};

template <typename InternetProtocol>
struct ResponseIcmpToIcmp
{
    Ipv4Header ipv4Header;
    IcmpHeader icmpHeader;
    TriggerPacket<InternetProtocol, IcmpHeader> triggerPacket;
};

template <typename InternetProtocol>
struct ResponseIcmpToTcp
{
    Ipv4Header ipv4Header;
    IcmpHeader icmpHeader;
    TriggerPacket<InternetProtocol, IcmpHeader> triggerPacket;
};

template <typename InternetProtocol>
struct ResponseIcmpToUdp
{
    Ipv4Header ipv4Header;
    IcmpHeader icmpHeader;
    TriggerPacket<InternetProtocol, IcmpHeader> triggerPacket;
};

} // namespace traceroute
