#pragma once
#include "SocketAddress.hpp"
#include <cstdint>
#include <sys/types.h>
#include <chrono>
#include "Packet.hpp"
namespace traceroute
{
struct ResponseInfo
{
    ssize_t size;
    SocketAddress client;
    int protocol;
};

class IDataSender
{
  public:
    virtual int sendPacket(const Packet & packet) = 0;
    virtual ResponseInfo receiveFrom(char *buffer, size_t bufferSize, std::chrono::milliseconds timeout) = 0;
    virtual void setTtlOnSendingSocket(int ttl) = 0;
    virtual ~IDataSender() = default;
};
} // namespace traceroute