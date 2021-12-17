#pragma once

#include <Traceroute/Probe.hpp>
#include <chrono>
#include <vector>

namespace traceroute {

class ITraceroute
{
  public:
    virtual std::vector<TracerouteResult> beginProbing(unsigned ttlBegin, unsigned ttlEnd, unsigned retries,
                                                       std::chrono::microseconds timeout) = 0;

    virtual ~ITraceroute() = default;
};

} // namespace traceroute