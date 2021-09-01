#pragma once
#include "../IPacketFactory.hpp"
namespace traceroute
{
    class UdpPacketFactory : public IPacketFactory
    {
    public:
        UdpPacketFactory(const SocketAddress &source,
                         const SocketAddress &destination, int dport);
        virtual std::unique_ptr<Packet> createPacket() final;
        ~UdpPacketFactory() final = default;
    private:
        SocketAddress mSource;
        SocketAddress mDestination;
        int mDport;
    };
}