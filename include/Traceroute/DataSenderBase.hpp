#pragma once

#include <cstdint>
#include "SocketAddress.hpp"
#include "IDataSender.hpp"
#include <vector>
namespace Traceroute
{

    class DataSenderBase : public IDataSender
    {
    public:
        int sendTo(const std::string &&buffer, const SocketAddress &receiver) override;
        int receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol) override;
        int getReceiveDelayMs() override;
        void setTtl(int ttl) override;
    protected:
        struct SocketInfo
        {
            int sfd;
            int protocol;
        };
        DataSenderBase(int family, const SocketAddress &sourceAddr, int delayMs);
        virtual int getSendingSocket() = 0;
        virtual std::vector<SocketInfo> getReceivingSockets() = 0;
        int mFamily;
        int mSfdIcmp;
        int mDelayMs;
    };
}