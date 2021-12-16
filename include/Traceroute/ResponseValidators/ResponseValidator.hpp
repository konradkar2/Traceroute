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
    bool isValid(const ResponseInfo & responseInfo, const char * response) override;

  protected:
    virtual bool validateFields(const ResponseInfo & responseInfo, const char * response) = 0;
    virtual bool validateProtocol(int protocol) = 0;
    virtual bool validateSize(size_t size) = 0;
};
} // namespace traceroute::responseValidators