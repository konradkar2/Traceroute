#pragma once
#include <string>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>
#include <cassert>
#include "ResponseBuilder.hpp"

namespace traceroute
{
    class IpHeaderBuilder
    {
    public:
        IpHeaderBuilder(ResponseBuilderV4 parent) : mParent(parent)
        {
           
        }
        IpHeaderBuilder &setSource(const SocketAddress &source)
        {
            mSource = source;
            return *this;
        }
        IpHeaderBuilder &setDestination(const SocketAddress &destination)
        {
            mDestination = destination;
            return *this;
        }
        IpHeaderBuilder &setIhl(int ihl)
        {
            mIhl = ihl;
            return *this;
        }
        ResponseBuilderV4 & next()
        {
            checkBuild();
            populateIpHeader();
            return mParent;
        }

    private:
        void checkBuild()
        {
            SocketAddress emptySocketAddress{};
            assert(mIhl > 0);
            assert(mDestination != emptySocketAddress);
            assert(mSource != emptySocketAddress);
        }
        Ipv4Header populateIpHeader()
        {
            Ipv4Header header;
            header.version = 4;
            header.ihl = mIhl;
            header.saddr = reinterpret_cast<const sockaddr_in *>(mSource.sockaddrP())->sin_addr.s_addr;
            header.daddr = reinterpret_cast<const sockaddr_in *>(mDestination.sockaddrP())->sin_addr.s_addr;
            return header;
        }
        SocketAddress mSource;
        SocketAddress mDestination;
        int mIhl;
        ResponseBuilderV4 & mParent;
    };
}