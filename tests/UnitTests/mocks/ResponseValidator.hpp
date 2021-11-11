#pragma once

#include <Traceroute/interface/IValidateResponse.hpp>
#include <gmock/gmock.h>

namespace traceroute
{
class ResponseValidatorMock : public IValidateResponse
{
  public:
    MOCK_METHOD(bool, validate, (const Packet &, const SocketAddress &, int, const char *, size_t), (override));
};
} // namespace traceroute