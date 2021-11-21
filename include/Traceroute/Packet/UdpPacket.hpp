#pragma once
#include "../HeaderTypes.hpp"
#include "../Packet.hpp"
#include "../SocketAddress.hpp"

namespace traceroute::packet {
class UdpPacket : public Packet
{
  public:
    UdpPacket(const SocketAddress &source, const SocketAddress &destination, int dport);
    const UdpHeader &GetUdpHeader() const;

    std::string serialize() const override;
    bool        validate(const ResponseInfo &responseInfo, const char *response) override;

  private:
    UdpHeader                          mUdpHeader;
    std::unique_ptr<IValidateResponse> mResponseValidator;
};

} // namespace traceroute::packet