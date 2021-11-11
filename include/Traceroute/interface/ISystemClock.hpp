#pragma once

#include <chrono>

namespace traceroute
{
class ISystemClock
{
  public:
    virtual std::chrono::steady_clock::time_point now() = 0;
    virtual ~ISystemClock() = default;
};

} // namespace traceroute