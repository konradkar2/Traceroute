#pragma once

#include "SocketAddress.hpp"
#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace traceroute {

class ProbeResult
{
  public:
    ProbeResult(bool success, std::chrono::microseconds waitedFor, std::optional<SocketAddress> responseSender)
        : mSuccess(success), mWaitedFor(waitedFor), mResponseSender(responseSender)
    {
    }
    bool success() const
    {
        return mSuccess;
    }
    std::chrono::microseconds waitedFor() const
    {
        return mWaitedFor;
    }
    const std::optional<SocketAddress> & responseSender() const
    {
        return mResponseSender;
    }

  private:
    bool                         mSuccess;
    std::chrono::microseconds    mWaitedFor;
    std::optional<SocketAddress> mResponseSender;
};

class TracerouteResult
{
  public:
    TracerouteResult(unsigned ttl, std::vector<ProbeResult> &&probeResults)
        : mTtl(ttl), mProbeResults(std::move(probeResults))
    {
    }
    unsigned ttl() const
    {
        return mTtl;
    }
    const std::vector<ProbeResult> &probeResults() const
    {
        return mProbeResults;
    }

  private:
    unsigned                 mTtl;
    std::vector<ProbeResult> mProbeResults;
};

ProbeResult failedProbe(std::chrono::microseconds waitedFor);
ProbeResult successProbe(std::chrono::microseconds waitedFor, const SocketAddress &responseSender);

std::string toString(const TracerouteResult &probe);
std::string toString(const std::vector<TracerouteResult> &probes);

} // namespace traceroute