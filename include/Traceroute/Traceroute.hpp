#pragma once

#include "SystemClock.hpp"
#include "Traceroute/interface/ITraceroute.hpp"
#include <Traceroute/interface/IDataSender.hpp>
#include <Traceroute/interface/IPacketFactory.hpp>
#include <Traceroute/interface/ISystemClock.hpp>
#include <chrono>
#include <memory>

namespace traceroute {

constexpr int                       BufferSize             = 4096;
constexpr std::chrono::microseconds MinTimeWaitForResponse = 10us;

class Traceroute : public ITraceroute
{
  public:
    Traceroute(IPacketFactory &packetFactory, IDataSender &dataSender, std::shared_ptr<ISystemClock> clock);
    virtual ~Traceroute() = default;

  protected:
    std::chrono::microseconds getTimePassedTillNow(std::chrono::steady_clock::time_point then) const;
    std::chrono::microseconds getTimeLeft(std::chrono::steady_clock::time_point then,
                                          std::chrono::microseconds             timeout) const;

  protected:
    IPacketFactory               &mPacketFactory;
    IDataSender                  &mDataSender;
    char                          mBuffer[BufferSize] = {0};
    std::shared_ptr<ISystemClock> mSystemClock;
};

} // namespace traceroute