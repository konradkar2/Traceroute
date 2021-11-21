#pragma once

#include <Traceroute/Packet/TcpPacket.hpp>
#include <Traceroute/ResponseValidators/ResponseValidator.hpp>

namespace traceroute::responseValidators {
class TcpToTcpResponseValidator : public ResponseValidator
{
  public:
    TcpToTcpResponseValidator(const packet::TcpPacket &tcpPacket);

  protected:
    bool validateFields(const ResponseInfo &responseInfo, const char *response) override;
    bool validateProtocol(int protocol) override;
    bool validateSize(size_t size) override;

  private:
    const packet::TcpPacket &mTcpPacket;
};
} // namespace traceroute::responseValidators
