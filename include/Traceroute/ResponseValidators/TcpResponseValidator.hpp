#pragma once

#include "Traceroute/ResponseValidators/ResponseValidator.hpp"
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/interface/IValidateResponse.hpp>
#include <cstdint>
#include <memory>
namespace traceroute {

namespace packet {
class TcpPacket;
}

namespace responseValidators {
class TcpResponseValidator : public IValidateResponse
{
  public:
    TcpResponseValidator(const packet::TcpPacket &tcpPacket);
    bool isValid(const ResponseInfo &responseInfo, const char *response) override;

  private:
    std::unique_ptr<IValidateResponse> mTcpToTcpValidator;
    std::unique_ptr<IValidateResponse> mIcmpToTcpValidator;
};
} // namespace responseValidators
} // namespace traceroute