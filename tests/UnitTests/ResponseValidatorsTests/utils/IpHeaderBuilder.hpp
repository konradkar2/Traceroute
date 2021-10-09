#pragma once
#include <string>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>
#include <cassert>


namespace traceroute
{
    class IpHeaderBuilder
    {
    public:
        IpHeaderBuilder &setSource(const SocketAddress &source)
        {
            mHeader.saddr = reinterpret_cast<const sockaddr_in *>(source.sockaddrP())->sin_addr.s_addr;
            return *this;
        }
        IpHeaderBuilder &setDestination(const SocketAddress &destination)
        {
            mHeader.daddr = reinterpret_cast<const sockaddr_in *>(destination.sockaddrP())->sin_addr.s_addr;
            return *this;
        }
        IpHeaderBuilder &setIhl(int ihl)
        {
            mHeader.ihl = ihl;
            return *this;
        }
        IpHeaderBuilder &setProtocol(int protocol)
        {
            mHeader.protocol = protocol;
            return *this;
        }
        Ipv4Header build()
        {
            finishAndVerify();
            auto mHeaderCopy = mHeader;
            mHeader = Ipv4Header{};
            return mHeaderCopy;
        }
    private:
        void finishAndVerify()
        {
            mHeader.version = 4;
            if(mHeader.ihl != 0)
                mHeader.ihl = 5;
            assert(mHeader.protocol != 0);
        }
        Ipv4Header mHeader;
        SocketAddress emptySocketAddress{};
    };
}