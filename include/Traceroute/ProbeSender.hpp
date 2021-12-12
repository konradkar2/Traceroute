#pragma once

#include "Packet.hpp"
#include "Probe.hpp"
#include "SystemClock.hpp"
#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/interface/IDataSender.hpp>
#include <Traceroute/interface/IPacketFactory.hpp>
#include <Traceroute/interface/ISystemClock.hpp>
#include <chrono>
#include <memory>

namespace traceroute {
const constexpr int                       BufferSize             = 4096;
const constexpr std::chrono::microseconds MinTimeWaitForResponse = 10us;

class ProbeSender
{
  public:
    ProbeSender(IPacketFactory &packetFactory, IDataSender &dataSender,
                std::shared_ptr<ISystemClock> clock = std::make_shared<SystemClock>());
    std::vector<ProbeResultContainer> beginProbing(int ttlBegin, int ttlEnd, int retries,
                                                   std::chrono::microseconds timeout);

  private:

    std::chrono::microseconds getTimePassedTillNow(std::chrono::steady_clock::time_point then) const;
    std::chrono::microseconds getTimeLeft(std::chrono::steady_clock::time_point then,
                                          std::chrono::microseconds             timeout) const;

  private:
    IPacketFactory               &mPacketFactory;
    IDataSender                  &mDataSender;
    char                          mBuffer[BufferSize] = {0};
    std::shared_ptr<ISystemClock> mSystemClock;
};

} // namespace traceroute