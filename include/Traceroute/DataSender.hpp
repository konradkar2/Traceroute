#pragma once

#include "SocketAddress.hpp"
#include <Traceroute/interface/IDataSender.hpp>
#include <Traceroute/interface/IProvideSockets.hpp>
#include <chrono>
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

namespace traceroute {

class DataSender : public IDataSender
{
  public:
    DataSender(std::vector<SocketExt> socketsExt, int family);
    bool trySending(const std::unique_ptr<Packet> &packet, int ttl, std::chrono::milliseconds timeout) override;
    std::optional<ResponseInfo> tryReceiving(char *buffer, std::size_t bufferSize,
                                             std::chrono::milliseconds timeout) override;

  private:
    void               setTtl(int ttl);
    std::optional<int> getAnySocketReadyToReceive(std::chrono::milliseconds timeout);

    std::map<int, int> fdToProtocol;
    std::vector<int>   mReceivingSockets;
    int                mSendingSocket;
    int                mFamily;
};
} // namespace traceroute