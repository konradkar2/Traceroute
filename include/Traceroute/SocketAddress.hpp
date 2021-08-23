#pragma once
#include <string>
#include <sys/socket.h>

namespace Traceroute
{
    class SocketAddress
    {
    public:
        SocketAddress() {}
        explicit SocketAddress(const sockaddr_storage &address);
        explicit SocketAddress(const std::string &address);
        int family() const;
        const sockaddr *sockaddrP() const;
        size_t size() const;
        std::string toString() const;
        bool operator==(const SocketAddress &rhs) const;
        bool isV4() const;
        bool isV6() const;

    private:
        struct sockaddr_storage mAddress;
    };
}