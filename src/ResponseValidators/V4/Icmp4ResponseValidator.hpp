#pragma once
#include <Traceroute/ResponseValidators/ResponseValidator.hpp>

namespace traceroute::responseValidators::v4
{
class Icmp4ResponseValidator : public ResponseValidator
{
  public:
    bool validateFields(const Packet &request, const SocketAddress &client, const char *response,
                        size_t responseSize) override;
    bool validateProtocol(int protocol) override;
    bool validateSize(size_t size) override;
};
} // namespace traceroute::responseValidators::v4
