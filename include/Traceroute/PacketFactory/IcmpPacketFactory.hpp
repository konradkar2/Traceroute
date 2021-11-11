#pragma once

#include <Traceroute/interface/IPacketFactory.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>


namespace traceroute
{
class IcmpPacketFactory : public IPacketFactory
{
  public:
    IcmpPacketFactory(const SocketAddress &source, const SocketAddress &destination);
    std::unique_ptr<Packet> createPacket() final;

  private:
    SocketAddress mSource;
    SocketAddress mDestination;
};

} // namespace traceroute