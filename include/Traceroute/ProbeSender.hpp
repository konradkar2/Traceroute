#pragma once

#include <chrono>
#include <memory>
#include "Packet.hpp"
#include "Probe.hpp"
#include "IDataSender.hpp"
#include "IValidateResponse.hpp"
#include "IPacketFactory.hpp"

namespace Traceroute
{
#define BUFLEN 4096
    class ProbeSender
    {
    public:
        ProbeSender(std::unique_ptr<IPacketFactory> packetFactory,
                    std::unique_ptr<IDataSender> dataSender,
                    std::unique_ptr<IValidateResponse> responseValidator);
        std::vector<ProbeResultContainer>  beginProbing(int ttlBegin, int ttlEnd, int retries,
                                          std::chrono::microseconds timeout);

    private:
        std::unique_ptr<IPacketFactory> mPacketFactory;
        std::unique_ptr<IDataSender> mDataSender;
        std::unique_ptr<IValidateResponse> mResponseValidator;
        char mSendingBuffer[BUFLEN] = {0};
        char mReceivingBuffer[BUFLEN] = {0};
    };

}