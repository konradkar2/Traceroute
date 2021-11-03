#pragma once
#include "Packet.hpp"
#include "SocketAddress.hpp"
#include <chrono>
#include <cstdint>
#include <sys/types.h>
#include <optional>
#include "ResponseInfo.hpp"

namespace traceroute
{
class IDataSender
{
  public:
    virtual int sendPacket(const Packet &packet) = 0;
    virtual std::optional<ResponseInfo> receiveFrom(char *buffer, size_t bufferSize,
                                                    std::chrono::milliseconds timeout) = 0;
    virtual void setTtlOnSendingSocket(int ttl) = 0;
    virtual ~IDataSender() = default;
};
} // namespace traceroute