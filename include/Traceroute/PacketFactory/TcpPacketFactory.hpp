#pragma once
#include "../IPacketFactory.hpp"
namespace Traceroute
{
    class TcpPacketFactory : public IPacketFactory
    {
    public:
        TcpPacketFactory(const SocketAddress &source,
                         const SocketAddress &destination, int dport);
        std::unique_ptr<Packet> createPacket() final;
        ~TcpPacketFactory() final = default;
    private:
        SocketAddress mSource;
        SocketAddress mDestination;
        int mDport;
    };
}