#include <Traceroute/SocketAddress.hpp>
#include <arpa/inet.h>
#include <cstring>
#include <netdb.h>
#include <stdexcept>
#include <string>
namespace traceroute
{
SocketAddress::SocketAddress(const sockaddr_storage &address)
{
    mAddress = address;
}
SocketAddress::SocketAddress(const std::string &address)
{
    struct addrinfo hint, *res = NULL;
    int ret;

    memset(&hint, '\0', sizeof hint);

    hint.ai_family = PF_UNSPEC;
    hint.ai_flags = AI_NUMERICHOST;

    ret = getaddrinfo(address.c_str(), NULL, &hint, &res);
    if (ret || (res->ai_family != AF_INET && res->ai_family != AF_INET6))
    {
        throw std::invalid_argument("Invalid address: " + address);
    }
    if (res->ai_family == AF_INET)
    {
        in_addr temp;
        if (inet_pton(AF_INET, address.c_str(), &temp) != 1)
        {
            throw std::runtime_error("Error occurred while converting ipv4 adress.");
        }

        mAddress.ss_family = AF_INET;
        ((sockaddr_in *)&mAddress)->sin_addr = temp;
        ((sockaddr_in *)&mAddress)->sin_port = 0;
    }
    else if (res->ai_family == AF_INET6)
    {
        in6_addr temp;
        if (inet_pton(AF_INET6, address.c_str(), &temp) != 1)
        {
            throw std::runtime_error("Error occurred while converting ipv6 adress.");
        }
        mAddress.ss_family = AF_INET6;
        ((sockaddr_in6 *)&mAddress)->sin6_addr = temp;
        ((sockaddr_in6 *)&mAddress)->sin6_port = 0;
    }
    freeaddrinfo(res);
}
int SocketAddress::family() const
{
    return (int)mAddress.ss_family;
}
size_t SocketAddress::size() const
{
    if (isV4())
        return sizeof(sockaddr_in);
    else
        return sizeof(sockaddr_in6);
}
const sockaddr *SocketAddress::sockaddrP() const
{
    return reinterpret_cast<const sockaddr *>(&mAddress);
}

bool SocketAddress::operator==(const SocketAddress &rhs) const
{
    bool isEqual = true;
    if (isV4())
        isEqual = ((sockaddr_in *)sockaddrP())->sin_addr.s_addr == ((sockaddr_in *)rhs.sockaddrP())->sin_addr.s_addr;
    else
    {
        for (int i = 0; i < 4; i++)
        {
            uint32_t tempA = ((sockaddr_in6 *)sockaddrP())->sin6_addr.__in6_u.__u6_addr32[i];
            uint32_t tempB = ((sockaddr_in6 *)rhs.sockaddrP())->sin6_addr.__in6_u.__u6_addr32[i];
            if (tempA != tempB)
            {
                isEqual = false;
                break;
            }
        }
    }
    return isEqual;
}
bool SocketAddress::operator!=(const SocketAddress &rhs) const
{
    return !operator==(rhs);
}
bool SocketAddress::isV4() const
{
    return family() == static_cast<int>(AF_INET);
}
bool SocketAddress::isV6() const
{
    return !isV4();
}
std::string SocketAddress::toString() const
{
    const int bufflen = INET6_ADDRSTRLEN;
    char buffer[bufflen];
    if (isV4())
        inet_ntop(AF_INET, &((sockaddr_in *)&mAddress)->sin_addr, buffer, bufflen);
    else
        inet_ntop(AF_INET6, &((sockaddr_in6 *)&mAddress)->sin6_addr, buffer, bufflen);
    return {buffer};
}

} // namespace traceroute
