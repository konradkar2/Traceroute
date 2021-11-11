#pragma once

#include <Traceroute/interface/ISystemClock.hpp>
#include <chrono>

namespace traceroute
{
class SystemClock : public ISystemClock
{
  public:
    std::chrono::steady_clock::time_point now() override;
};

} // namespace traceroute