#pragma once

#include <cstdint>
#include "SocketAddress.hpp"
#include "IDataSender.hpp"
#include <vector>
#include <chrono>
#include <poll.h>
namespace traceroute
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
        DataSenderBase(const SocketAddress &sourceAddr, const SocketInfo &transportProtocolSocket, std::chrono::milliseconds pollingTimeout);
        int sendTo(const std::string &&buffer, const SocketAddress &receiver) override;
        int receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol) override;
        void setTtlOnSendingSocket(int ttl) override;

    private:
        void handleTransportProtocolSocket(const SocketInfo &transportProtocolSocket);
        void initializePollingFds();
        ssize_t getIndexOfAnySocketReadyToReceive();
        std::vector<SocketInfo> mReceivingSockets;
        std::chrono::milliseconds mPollingTimeout;
        SocketInfo mSendingSocket;
        int mFamily;
        struct pollfd mPollFds[2];
    };
}