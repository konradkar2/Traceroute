#pragma once

#include "IDataSender.hpp"
#include "IPacketFactory.hpp"
#include "IValidateResponse.hpp"
#include "Packet.hpp"
#include "Probe.hpp"
#include <chrono>
#include <memory>

namespace traceroute
{
const constexpr int BufferSize = 4096;
const constexpr std::chrono::microseconds MinTimeWaitForResponse = 100us;
class ProbeSender
{
  public:
    ProbeSender(std::unique_ptr<IPacketFactory> packetFactory, std::unique_ptr<IDataSender> dataSender,
                std::unique_ptr<IValidateResponse> responseValidator);
    std::vector<ProbeResultContainer> beginProbing(int ttlBegin, int ttlEnd, int retries,
                                                   std::chrono::microseconds timeout);

  private:
    std::unique_ptr<IPacketFactory> mPacketFactory;
    std::unique_ptr<IDataSender> mDataSender;
    std::unique_ptr<IValidateResponse> mResponseValidator;
    char mBuffer[BufferSize] = {0};
};

} // namespace traceroute