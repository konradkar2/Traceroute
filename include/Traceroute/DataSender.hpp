#pragma once

#include "IDataSender.hpp"
#include "IProvideSockets.hpp"
#include "SocketAddress.hpp"
#include <chrono>
#include <cstdint>
#include <map>
#include <vector>

namespace traceroute {

class DataSender : public IDataSender
{
  public:
    DataSender(std::vector<SocketExt> socketsExt, int family);
    int sendPacket(const Packet &packet) override;
    std::optional<ResponseInfo> receiveFrom(char *buffer, size_t bufferSize,
                                            std::chrono::milliseconds timeout) override;
    void setTtlOnSendingSocket(int ttl) override;

  private:
    std::optional<int> getAnySocketReadyToReceive(std::chrono::milliseconds timeout);

    std::map<int, int> fdToProtocol;
    std::vector<int> mReceivingSockets;
    int mSendingSocket;
    int mFamily;
};
} // namespace traceroute