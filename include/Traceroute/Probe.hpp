#pragma once
#include "SocketAddress.hpp"
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace traceroute
{

class ProbeResultContainer
{
  public:
    struct ProbeResult
    {
        bool success;
        std::chrono::microseconds waitedFor;
    };
    explicit ProbeResultContainer(int ttl);

    void addFailedProbe(std::chrono::microseconds waitedFor);
    void addSuccessfulProbe(std::chrono::microseconds waitedFor);

    void setResponseAddr(const SocketAddress &responseSender);
    std::optional<SocketAddress> GetResponseAddr() const;

    std::string toString() const;
    const std::vector<ProbeResult> &getResults() const;

  private:
    std::vector<ProbeResult> mProbeResults;
    std::optional<SocketAddress> mResponseSender;
    int mTtl;
};

} // namespace traceroute