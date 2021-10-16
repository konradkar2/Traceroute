#pragma once
#include "../IPacketFactory.hpp"
namespace traceroute
{
class UdpPacketFactory : public IPacketFactory
{
  public:
    UdpPacketFactory(const SocketAddress &source, const SocketAddress &destination, int dport);
    std::unique_ptr<Packet> createPacket() final;

  private:
    SocketAddress mSource;
    SocketAddress mDestination;
    int mDport;
};
} // namespace traceroute