#pragma once

#include "Traceroute/ISystemClock.hpp"
#include <chrono>

namespace traceroute {

using namespace std::chrono;
class SystemClockFake : public ISystemClock
{
  public:
    SystemClockFake()
    {
        mTimePoint = steady_clock::now();
    }
    steady_clock::time_point now() override
    {
        return mTimePoint;
    }
    void advanceBy(std::chrono::microseconds duration)
    {
        mTimePoint += duration;
    }

  private:
    std::chrono::steady_clock::time_point mTimePoint;
};
} // namespace traceroute
