#pragma once
#include <string>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>
#include "IpHeaderBuilder.hpp"
#include "IParent.hpp"
namespace traceroute
{

    enum class Protocol
    {
        Icmp,
        Tcp,
        Udp,
    };
    class IcmpHeaderBuilder;
    class TcpHeaderBuilder;
    class UdpHeaderBuilder;

    class ResponseBuilderV4 : public IParent
    {
    public:
        IpHeaderBuilder buildIpHeader()
        {
            return IpHeaderBuilder{*this};
        }

    private:
        Ipv4Header mIpHeader;
    };

}