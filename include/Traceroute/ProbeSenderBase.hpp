#pragma once

#include "Probe.hpp"
#include "IDataSender.hpp"
#include <chrono>
#include "Packet.hpp"
namespace Traceroute
{
#define BUFLEN 4096
    class ProbeSenderBase
    {
    protected:
        ProbeSenderBase(IDataSender *dataSender);
        ProbeResultContainer BeginProbing(const Packet *packet, int ttl, int retries, std::chrono::microseconds timeout);

        virtual void sendPacket();
        virtual bool isResponseValid(const SocketAddress &client, const int &protocol) = 0;

        char *getReceiveBuf();
        int getTtl() const;
        virtual ~ProbeSenderBase(){};

    private:
        void setTtl(int ttl);
        const Packet *mPPacket;
        IDataSender *mPDataSender;
        int mTtl;
        char mSendingBuffer[BUFLEN] = {0};
        char mReceivingBuffer[BUFLEN] = {0};
        ProbeSenderBase();
    };

}