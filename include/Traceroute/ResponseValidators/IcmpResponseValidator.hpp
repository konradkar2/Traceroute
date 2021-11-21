#pragma once

#include "Traceroute/ResponseInfo.hpp"
#include "Traceroute/ResponseValidators/ResponseValidator.hpp"
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <cstdint>
#include <memory>

namespace traceroute {

namespace packet {
class IcmpPacket;
}

namespace responseValidators {
class IcmpResponseValidator : public ResponseValidator
{
  public:
    IcmpResponseValidator(const packet::IcmpPacket &icmpPacket);

  protected:
    bool validateFields(const ResponseInfo &responseInfo, const char *response) override;
    bool validateProtocol(int protocol) override;
    bool validateSize(size_t size) override;

  private:
    const packet::IcmpPacket &mIcmpPacket;
};
} // namespace responseValidators
} // namespace traceroute