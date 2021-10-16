#pragma once

#include "IProvideSocket.hpp"
#include "SocketAddress.hpp"
#include <chrono>
#include <cstdint>
#include <map>
#include <poll.h>
#include <vector>
#include "IDataSender.hpp"


namespace traceroute
{

class DataSender : public IDataSender
{
  public:
    DataSender(std::vector<Socket> sockets, int family, std::chrono::milliseconds pollingTimeout);
    int sendTo(const std::string &&buffer, const SocketAddress &receiver) override;
    int receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol) override;
    void setTtlOnSendingSocket(int ttl) override;

  private:
    int getAnySocketReadyToReceive();

    std::chrono::milliseconds mPollingTimeout;
    std::map<int, int> sfdToProtocol;
    std::vector<int> mReceivingSockets;
    int mSendingSocket;
    int mFamily;
};
} // namespace traceroute