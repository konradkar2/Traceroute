#pragma once

#include <chrono>
#include <memory>
#include "Packet.hpp"
#include "Probe.hpp"
#include "IDataSender.hpp"
#include "IValidateResponse.hpp"

namespace Traceroute
{
#define BUFLEN 4096
    class ProbeSender
    {
    public:
        ProbeSender(std::unique_ptr<IDataSender> && dataSender, std::unique_ptr<IValidateResponse> && responseValidator);
        ProbeResultContainer beginProbing(const Packet * packet, int ttl, int retries, std::chrono::microseconds timeout);
    private:
        void sendPacket();
        void setTtl(int ttl);
        const Packet * mPacket;
        std::unique_ptr<IDataSender> mDataSender;
        std::unique_ptr<IValidateResponse> mResponseValidator;
        int mTtl;
        char mSendingBuffer[BUFLEN] = {0};
        char mReceivingBuffer[BUFLEN] = {0};
    };

}