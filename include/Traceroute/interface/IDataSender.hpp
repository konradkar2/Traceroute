#pragma once
#include <Traceroute/ResponseInfo.hpp>
#include <Traceroute/Packet.hpp>
#include <chrono>
#include <cstdint>
#include <sys/types.h>
#include <optional>


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