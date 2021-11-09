#pragma once

#include "IDataSender.hpp"
#include "IPacketFactory.hpp"
#include "Packet.hpp"
#include "Probe.hpp"
#include "SystemClock.hpp"
#include "Traceroute/ISystemClock.hpp"
#include <Traceroute/IValidateResponse.hpp>
#include <chrono>
#include <memory>

namespace traceroute {
const constexpr int BufferSize = 4096;
const constexpr std::chrono::microseconds MinTimeWaitForResponse = 10us;
class ProbeSender
{
  public:
    ProbeSender(IPacketFactory &packetFactory, IDataSender &dataSender, IValidateResponse &responseValidator,
                std::shared_ptr<ISystemClock> clock = std::make_shared<SystemClock>());
    std::vector<ProbeResultContainer> beginProbing(int ttlBegin, int ttlEnd, int retries,
                                                   std::chrono::microseconds timeout);

  private:
    std::chrono::microseconds getTimePassedTillNow(std::chrono::steady_clock::time_point then) const;
    std::chrono::microseconds getTimeLeft(std::chrono::steady_clock::time_point then,
                                          std::chrono::microseconds timeout) const ;

  private:
    IPacketFactory &mPacketFactory;
    IDataSender &mDataSender;
    IValidateResponse &mResponseValidator;
    char mBuffer[BufferSize] = {0};
    std::shared_ptr<ISystemClock> mSystemClock;
};

} // namespace traceroute