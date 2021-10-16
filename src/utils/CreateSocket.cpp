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

Socket createIcmpSocket(const SocketAddress &addressToBind)
{
    Socket sockt;
    sockt.protocol = addressToBind.isV4() ? (int)IPPROTO_ICMP : (int)IPPROTO_ICMPV6;
    sockt.sfd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, sockt.protocol);

    Bind(sockt.sfd, addressToBind);
    if (addressToBind.isV6())
    {
        struct icmp6_filter filter;
        ICMP6_FILTER_SETBLOCKALL(&filter);
        ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &filter);
        ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &filter);
        ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &filter);
        if (setsockopt(sockt.sfd, IPPROTO_ICMPV6, ICMP6_FILTER, &filter, sizeof(filter)) < 0)
        {
            throw std::runtime_error("Error occured while setting icmpv6 filter: " + std::string(strerror(errno)));
        }
    }
    return sockt;
}

Socket createTcpSocket(const SocketAddress &addressToBind)
{
    int sfd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, IPPROTO_TCP);
    Bind(sfd, addressToBind);
    if (addressToBind.isV6())
    {
        const int TCP_CHECKSUM_OFFSET = 16;
        if ((setsockopt(sfd, IPPROTO_IPV6, IPV6_CHECKSUM, &TCP_CHECKSUM_OFFSET, sizeof(TCP_CHECKSUM_OFFSET))) < 0)
        {
            throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
        }
    }
    return {sfd, IPPROTO_TCP, false, false};
}

Socket createUdpSocket(const SocketAddress &addressToBind)
{
    int sfd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, IPPROTO_UDP);
    Bind(sfd, addressToBind);
    if (addressToBind.isV6())
    {
        int offset = 6;
        if ((setsockopt(sfd, IPPROTO_IPV6, IPV6_CHECKSUM, &offset, sizeof(offset))) < 0)
        {
            throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
        }
    }
    return {sfd, IPPROTO_UDP, false, false};
}
} // namespace traceroute::utils