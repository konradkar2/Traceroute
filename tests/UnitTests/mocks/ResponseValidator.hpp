#pragma once

#include <Traceroute/interface/IValidateResponse.hpp>
#include <gmock/gmock.h>

namespace traceroute {
class ResponseValidatorMock : public IValidateResponse
{
  public:
    MOCK_METHOD(bool, isValid, (const ResponseInfo &, const char *), (override));
};
} // namespace traceroute