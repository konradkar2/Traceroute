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
#define BUFLEN 4096
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
    char mBuffer[BUFLEN] = {0};
};

} // namespace traceroute