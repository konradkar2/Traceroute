#pragma once
#include <vector>
#include <string>
#include <chrono>
#include "SocketAddress.hpp"

namespace traceroute
{

    class ProbeResultContainer
    {
    public:
        struct ProbeResult
        {
            bool success;
            std::chrono::microseconds receivedAfter;
        };
        explicit ProbeResultContainer(int ttl);
        void addFailedProbe(std::chrono::microseconds waitedFor);
        void addSuccessfulProbe(std::chrono::microseconds waitedFor);
        void setResponseAddr(const SocketAddress & responseSender);
        const SocketAddress &GetResponseAddr() const;

        std::string toString() const;
        const std::vector<ProbeResult> &getResults() const;

    private:
        std::vector<ProbeResult> mProbeResults;
        SocketAddress mResponseSender;
        int mTtl;
    };

}