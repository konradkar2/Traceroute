#pragma once

#include <Traceroute/interface/IPacketFactory.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>

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