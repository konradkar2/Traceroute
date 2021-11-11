#pragma once
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <cstdint>

namespace traceroute
{
class IValidateResponse
{
  public:
    virtual bool validate(const Packet &request, const SocketAddress &client, int protocol, const char *response,
                          size_t responseSize) = 0;
    virtual ~IValidateResponse() = default;
};
} // namespace traceroute::responseValidators