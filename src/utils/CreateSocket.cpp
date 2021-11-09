#include "CreateSocket.hpp"
#include "Bind.hpp"
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace traceroute::utils {
namespace {

void bindToIpAddressAndInterface(int fd, const SocketAddress &addressToBind, std::optional<std::string> ifaceName)
{
    if (ifaceName)
    {
        bindInterface(fd, ifaceName.value());
    }
    bindIpAddress(fd, addressToBind);
}
} // namespace

Socket setupIcmpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName)
{
    const int protocol = addressToBind.isV4() ? static_cast<int>(IPPROTO_ICMP) : static_cast<int>(IPPROTO_ICMPV6);
    int fd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, protocol);

    bindToIpAddressAndInterface(fd, addressToBind, ifaceName);
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

Socket setupTcpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName)
{
    constexpr int protocol = IPPROTO_TCP;
    int fd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, protocol);

    bindToIpAddressAndInterface(fd, addressToBind, ifaceName);
    if (addressToBind.isV6())
    {
        constexpr int TcpChecksumOffset = 16;
        if ((setsockopt(fd, IPPROTO_IPV6, IPV6_CHECKSUM, &TcpChecksumOffset, sizeof(TcpChecksumOffset))) < 0)
        {
            throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
        }
    }
    return Socket{fd, protocol};
}

Socket setupUdpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName)
{
    constexpr int protocol = IPPROTO_UDP;
    int fd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, protocol);

    bindToIpAddressAndInterface(fd, addressToBind, ifaceName);
    if (addressToBind.isV6())
    {
        constexpr int UdpChecksumOffset = 6;
        if ((setsockopt(fd, IPPROTO_IPV6, IPV6_CHECKSUM, &UdpChecksumOffset, sizeof(UdpChecksumOffset))) < 0)
        {
            throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
        }
    }
    return Socket{fd, protocol};
}
} // namespace traceroute::utils