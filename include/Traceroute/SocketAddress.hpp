#pragma once
#include <string>
#include <sys/socket.h>

namespace traceroute
{
    class SocketAddress
    {
    public:
        SocketAddress() {}
        explicit SocketAddress(const sockaddr_storage &address);
        explicit SocketAddress(const std::string &address);
        int family() const;
        const sockaddr *sockaddrP() const;
        std::string toString() const;
        size_t size() const;       
        bool isV4() const;
        bool isV6() const;
        bool operator==(const SocketAddress &rhs) const;
        bool operator!=(const SocketAddress &rhs) const;
    private:
        struct sockaddr_storage mAddress;
    };
}