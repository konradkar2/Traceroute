#pragma once

#include <string>
#include <sys/socket.h>

namespace traceroute {
class SocketAddress
{
  public:
    explicit SocketAddress(const sockaddr_storage &address);
    explicit SocketAddress(const std::string &address);
    int             family() const;
    const sockaddr *sockaddrP() const;
    std::string     toString() const;
    unsigned        size() const;
    bool            isV4() const;
    bool            isV6() const;
    bool            operator==(const SocketAddress &rhs) const;
    bool            operator!=(const SocketAddress &rhs) const;

    SocketAddress(const SocketAddress &) = default;
    SocketAddress(SocketAddress &&)      = default;
    SocketAddress &operator=(const SocketAddress &) = default;
    SocketAddress &operator=(SocketAddress &&) = default;

  private:
    sockaddr_storage mAddress;
};
} // namespace traceroute