#pragma once

#include "Probe.hpp"
#include "IDataSender.hpp"
#include "IValidateResponse.hpp"
#include <chrono>
#include "Packet.hpp"
namespace Traceroute
{
#define BUFLEN 4096
    class ProbeSender
    {
    public:
        ProbeSender(IDataSender *dataSender, IValidateResponse * responseValidator);
        ProbeResultContainer beginProbing(const Packet *packet, int ttl, int retries, std::chrono::microseconds timeout);
    private:
        void sendPacket();
        void setTtl(int ttl);
        const Packet * mPacket;
        IDataSender *mDataSender;
        IValidateResponse * mResponseValidator;
        int mTtl;
        char mSendingBuffer[BUFLEN] = {0};
        char mReceivingBuffer[BUFLEN] = {0};
    };

}