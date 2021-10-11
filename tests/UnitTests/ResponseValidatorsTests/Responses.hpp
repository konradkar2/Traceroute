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
    InternetProtocol ipHeader;
    IcmpHeader icmpHeader;
    TriggerPacket<InternetProtocol, IcmpHeader> triggerPacket;
};

template <typename InternetProtocol>
struct ResponseIcmpToTcp
{
    InternetProtocol ipHeader;
    IcmpHeader icmpHeader;
    TriggerPacket<InternetProtocol, IcmpHeader> triggerPacket;
};

template <typename InternetProtocol>
struct ResponseIcmpToUdp
{
    InternetProtocol ipHeader;
    IcmpHeader icmpHeader;
    TriggerPacket<InternetProtocol, IcmpHeader> triggerPacket;
};

} // namespace traceroute
