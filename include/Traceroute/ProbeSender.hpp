#pragma once

#include "IDataSender.hpp"
#include "IPacketFactory.hpp"
#include "Packet.hpp"
#include "Probe.hpp"
#include <Traceroute/IValidateResponse.hpp>
#include <chrono>
#include <memory>

namespace traceroute
{
const constexpr int BufferSize = 4096;
const constexpr std::chrono::microseconds MinTimeWaitForResponse = 10us;
class ProbeSender
{
  public:
    ProbeSender(IPacketFactory &packetFactory, IDataSender &dataSender, IValidateResponse &responseValidator);
    std::vector<ProbeResultContainer> beginProbing(int ttlBegin, int ttlEnd, int retries,
                                                   std::chrono::microseconds timeout);

  private:
    IPacketFactory &mPacketFactory;
    IDataSender &mDataSender;
    IValidateResponse &mResponseValidator;
    char mBuffer[BufferSize] = {0};
};

} // namespace traceroute