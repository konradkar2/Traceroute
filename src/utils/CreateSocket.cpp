#include "CreateSocket.hpp"
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>

namespace traceroute::utils
{
namespace
{
void Bind(int sfd, const SocketAddress &address)
{
    if ((bind(sfd, address.sockaddrP(), address.size())) < 0)
    {
        throw std::runtime_error("Could not bind address: " + address.toString());
    }
}
} // namespace

Socket createIcmpRawSocket(const SocketAddress &addressToBind)
{
    const int protocol = addressToBind.isV4() ? static_cast<int>(IPPROTO_ICMP) : static_cast<int>(IPPROTO_ICMPV6);
    int fd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, protocol);

    Bind(fd, addressToBind);
    if (addressToBind.isV6())
    {
        struct icmp6_filter filter;
        ICMP6_FILTER_SETBLOCKALL(&filter);
        ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &filter);
        ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &filter);
        ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &filter);
        if (setsockopt(fd, IPPROTO_ICMPV6, ICMP6_FILTER, &filter, sizeof(filter)) < 0)
        {
            throw std::runtime_error("Error occured while setting icmpv6 filter: " + std::string(strerror(errno)));
        }
    }
    return Socket{fd, protocol};
}

Socket createTcpRawSocket(const SocketAddress &addressToBind)
{
    constexpr int protocol = IPPROTO_TCP;
    int fd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, protocol);
    Bind(fd, addressToBind);
    if (addressToBind.isV6())
    {
        const int TCP_CHECKSUM_OFFSET = 16;
        if ((setsockopt(fd, IPPROTO_IPV6, IPV6_CHECKSUM, &TCP_CHECKSUM_OFFSET, sizeof(TCP_CHECKSUM_OFFSET))) < 0)
        {
            throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
        }
    }
    return Socket{fd, protocol};
}

Socket createUdpRawSocket(const SocketAddress &addressToBind)
{
    constexpr int protocol = IPPROTO_UDP;
    int fd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, protocol);
    Bind(fd, addressToBind);
    if (addressToBind.isV6())
    {
        int offset = 6;
        if ((setsockopt(fd, IPPROTO_IPV6, IPV6_CHECKSUM, &offset, sizeof(offset))) < 0)
        {
            throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
        }
    }
    return Socket{fd, protocol};
}
} // namespace traceroute::utils