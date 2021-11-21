#pragma once
#include "Traceroute/HeaderTypes.hpp"
#include "Traceroute/Packet.hpp"
#include "Traceroute/ResponseValidators/TcpResponseValidator.hpp"
#include "Traceroute/SocketAddress.hpp"

namespace traceroute::packet {

class TcpPacket : public Packet
{
  public:
    TcpPacket(const SocketAddress &source, const SocketAddress &destination, int dport);
    const TcpHeader &getTcpHeader() const;

    std::string serialize() const override;
    bool        validate(const ResponseInfo &responseInfo, const char *response) override;

  private:
    TcpHeader                                mTcpHeader;
    responseValidators::TcpResponseValidator mResponseValidator;
};
} // namespace traceroute::packet