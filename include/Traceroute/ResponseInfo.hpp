#pragma once

#include "SocketAddress.hpp"
#include <cstdint>
#include <sys/types.h>

namespace traceroute
{

class ResponseInfo
{
  public:
    ResponseInfo(SocketAddress &&client, int protocol, ssize_t size);

    const SocketAddress &client();
    int protocol();
    ssize_t size();

  private:
    SocketAddress mClient;
    int mProtocol;
    ssize_t mSize;
};

} // namespace traceroute
