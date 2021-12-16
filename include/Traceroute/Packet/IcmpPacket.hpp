#pragma once

#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <memory>

namespace traceroute::packet {

class IcmpPacket : public Packet
{
  public:
    static std::unique_ptr<IcmpPacket> CreateIcmp4Packet(const SocketAddress &source, const SocketAddress &destination);
    static std::unique_ptr<IcmpPacket> CreateIcmp6Packet(const SocketAddress &source, const SocketAddress &destination);
    const IcmpHeader                  &GetIcmpHeader() const;

    std::string serialize() const override;
    bool        isValid(const ResponseInfo &responseInfo, const char *response) override;

  private:
    IcmpPacket(const SocketAddress &source, const SocketAddress &destination);
    IcmpHeader                                mIcmpHeader;
};
} // namespace traceroute::packet