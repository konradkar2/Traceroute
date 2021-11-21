#pragma once
#include "../HeaderTypes.hpp"
#include "../Packet.hpp"
#include "../SocketAddress.hpp"

namespace traceroute::packet {

class TcpPacket : public Packet
{
  public:
    TcpPacket(const SocketAddress &source, const SocketAddress &destination, int dport);
    const TcpHeader &getTcpHeader() const;

    std::string serialize() const override;
    bool        validate(const ResponseInfo &responseInfo, const char *response) override;

  private:
    TcpHeader                          mTcpHeader;
    std::unique_ptr<IValidateResponse> mResponseValidator;
};
} // namespace traceroute::packet