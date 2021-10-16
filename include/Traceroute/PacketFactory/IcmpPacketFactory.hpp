#pragma once
#include "../IPacketFactory.hpp"
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