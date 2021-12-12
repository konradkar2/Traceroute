#pragma once
#include <Traceroute/Packet.hpp>
#include <Traceroute/ResponseInfo.hpp>
#include <chrono>
#include <cstdint>
#include <optional>
#include <sys/types.h>

namespace traceroute {
class IDataSender
{
  public:
    virtual bool trySending(const std::unique_ptr<Packet> &packet, int ttl, std::chrono::milliseconds timeout) = 0;
    virtual std::optional<ResponseInfo> tryReceiving(char *buffer, std::size_t bufferSize,
                                                     std::chrono::milliseconds timeout)                        = 0;
    virtual ~IDataSender() = default;
};
} // namespace traceroute