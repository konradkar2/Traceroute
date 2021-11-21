#pragma once

#include "SocketAddress.hpp"
#include <cstdint>
#include <sys/types.h>

namespace traceroute {

class ResponseInfo
{
  public:
    ResponseInfo(const SocketAddress &client, int protocol, size_t size);

    const SocketAddress &client() const;
    int                  protocol() const;
    size_t               size() const;

  private:
    SocketAddress mClient;
    int           mProtocol;
    size_t        mSize;
};

} // namespace traceroute
