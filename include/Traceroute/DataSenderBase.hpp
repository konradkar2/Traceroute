#pragma once

#include <cstdint>
#include "SocketAddress.hpp"
#include "IDataSender.hpp"
#include <vector>
#include <chrono>
#include <poll.h>
namespace Traceroute
{
    struct SocketInfo
    {
        int sfd;
        int protocol;
        bool isDesignatedToReceive;
    };
    class DataSenderBase : public IDataSender
    {
    public:
        DataSenderBase(const SocketAddress &sourceAddr, const SocketInfo &transportProtocolSocket, std::chrono::milliseconds receiveTimeout);
        int sendTo(const std::string &&buffer, const SocketAddress &receiver) override;
        int receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol) override;
        void setTtl(int ttl) override;

    private:
        void handleTransportProtocolSocket(const SocketInfo &transportProtocolSocket);
        void initializePollingFds();
        ssize_t getIndexOfAnySocketReadyToReceive();
        std::vector<SocketInfo> mReceivingSockets;
        SocketInfo mSendingSocket;
        int mFamily;
        std::chrono::milliseconds mReceiveTimeout;

        struct pollfd mPollFds[2];
    };
}