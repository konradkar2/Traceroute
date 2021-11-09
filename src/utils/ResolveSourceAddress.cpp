#include "ResolveSourceAddress.hpp"
#include "Bind.hpp"
#include <cstring>
#include <netinet/in.h>
#include <optional>
#include <stdexcept>
#include <sys/socket.h>

namespace traceroute::utils {

SocketAddress resolveSourceAddress(const SocketAddress &destination, std::optional<std::string> ifaceName)
{
    int sfd = socket(destination.family(), SOCK_DGRAM, IPPROTO_UDP);
    if (ifaceName)
    {
        bindInterface(sfd, ifaceName.value());
    }
    if (connect(sfd, destination.sockaddrP(), destination.size()) < 0)
    {
        throw std::runtime_error("Failed to resolve source address. Error: connect() failed");
    };

    sockaddr_storage sockaddrStorage;
    socklen_t len;
    if (getsockname(sfd, (sockaddr *)&sockaddrStorage, &len) < 0)
    {
        throw std::runtime_error("Failed to resolve source address. Error: getsockname() failed");
    }

    return SocketAddress{sockaddrStorage};
}

} // namespace traceroute::utils
