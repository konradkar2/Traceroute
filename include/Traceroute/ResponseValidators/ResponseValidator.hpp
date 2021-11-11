#pragma once
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <cstdint>
#include <Traceroute/interface/IValidateResponse.hpp>

namespace traceroute::responseValidators
{
class ResponseValidator : public IValidateResponse
{
  public:
    bool validate(const Packet &request, const SocketAddress &client, int protocol, const char *response,
                  size_t responseSize) override;

  protected:
    virtual bool validateFields(const Packet &request, const SocketAddress &client, const char *response,
                                size_t responseSize) = 0;
    virtual bool validateProtocol(int protocol) = 0;
    virtual bool validateSize(size_t size) = 0;
};
} // namespace traceroute::responseValidators