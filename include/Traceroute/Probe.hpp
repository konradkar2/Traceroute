#pragma once

#include "SocketAddress.hpp"
#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace traceroute {

struct ProbeResult
{
    const bool                         success;
    const std::chrono::microseconds    waitedFor;
    const std::optional<SocketAddress> responseSender;
};

struct TracerouteResult
{
    const unsigned                 ttl;
    const std::vector<ProbeResult> probeResults;
};

ProbeResult failedProbe(std::chrono::microseconds waitedFor);
ProbeResult successProbe(std::chrono::microseconds waitedFor, const SocketAddress &responseSender);

std::string toString(const TracerouteResult &probe);
std::string toString(const std::vector<TracerouteResult> &probes);

} // namespace traceroute