#pragma once

#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/interface/IValidateResponse.hpp>
#include <memory>

namespace traceroute::packet {

class IcmpPacket : public Packet
{
  public:
    static IcmpPacket CreateIcmp4Packet(const SocketAddress &source, const SocketAddress &destination);
    static IcmpPacket CreateIcmp6Packet(const SocketAddress &source, const SocketAddress &destination);
    const IcmpHeader &GetIcmpHeader() const;

    std::string serialize() const override;
    bool        validate(const ResponseInfo &responseInfo, const char *response) override;

  private:
    IcmpPacket(const SocketAddress &source, const SocketAddress &destination);
    std::unique_ptr<IValidateResponse> mResponseValidator;
    IcmpHeader                         mIcmpHeader;
};
} // namespace traceroute::packet