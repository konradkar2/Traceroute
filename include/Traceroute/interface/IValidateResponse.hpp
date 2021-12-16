#pragma once
#include "Traceroute/ResponseInfo.hpp"

namespace traceroute {
class IValidateResponse
{
  public:
    virtual bool isValid(const ResponseInfo &responseInfo, const char *response) = 0;
    virtual ~IValidateResponse()                                                 = default;
};
} // namespace traceroute