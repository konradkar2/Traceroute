#pragma once
#include "Traceroute/HeaderTypes.hpp"
#include "Traceroute/Packet.hpp"
#include "Traceroute/SocketAddress.hpp"

namespace traceroute::packet {
class UdpPacket : public Packet
{
  public:
    UdpPacket(const SocketAddress &source, const SocketAddress &destination, int dport);
    const UdpHeader &GetUdpHeader() const;

    std::string serialize() const override;
    bool        isValid(const ResponseInfo &responseInfo, const char *response) override;

  private:
    UdpHeader mUdpHeader;
};

} // namespace traceroute::packet