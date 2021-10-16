#pragma once
#include "../IPacketFactory.hpp"
namespace traceroute
{
class TcpPacketFactory : public IPacketFactory
{
  public:
    TcpPacketFactory(const SocketAddress &source, const SocketAddress &destination, int dport);
    std::unique_ptr<Packet> createPacket() final;

  private:
    SocketAddress mSource;
    SocketAddress mDestination;
    int mDport;
};
} // namespace traceroute