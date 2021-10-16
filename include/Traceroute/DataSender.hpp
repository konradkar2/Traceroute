#pragma once

#include "IProvideSocket.hpp"
#include "SocketAddress.hpp"
#include <chrono>
#include <cstdint>
#include <map>
#include <poll.h>
#include <vector>
#include "IDataSender.hpp"
#include "IProvideSocket.hpp"

namespace traceroute
{

class DataSender : public IDataSender
{
  public:
    DataSender(std::vector<Socket> sockets, int family);
    int sendTo(const std::string &&buffer, const SocketAddress &receiver) override;
    ResponseInfo receiveFrom(char *buffer, size_t bufferSize, std::chrono::milliseconds timeout) override;
    void setTtlOnSendingSocket(int ttl) override;

  private:
    int getAnySocketReadyToReceive(std::chrono::milliseconds timeout);

    std::map<int, int> sfdToProtocol;
    std::vector<int> mReceivingSockets;
    int mSendingSocket;
    int mFamily;
};
} // namespace traceroute