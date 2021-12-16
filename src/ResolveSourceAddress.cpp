#include "utils/Bind.hpp"
#include <Traceroute/ResolveSourceAddress.hpp>
#include <cstring>
#include <netinet/in.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace traceroute {

SocketAddress resolveSourceAddress(const SocketAddress &destination,  const std::optional<std::string> & ifaceName)
{
    int sfd = socket(destination.family(), SOCK_DGRAM, IPPROTO_UDP);
    if (ifaceName)
    {
        utils::bindInterface(sfd, ifaceName.value());
    }
    if (connect(sfd, destination.sockaddrP(), destination.size()) < 0)
    {
        throw std::runtime_error("Failed to resolve source address. Error: connect() failed");
    };

    sockaddr_storage sockaddrStorage;
    socklen_t        len;
    if (getsockname(sfd, (sockaddr *)&sockaddrStorage, &len) < 0)
    {
        throw std::runtime_error("Failed to resolve source address. Error: getsockname() failed: " + std::to_string(errno));
    }
    close(sfd);
    return SocketAddress{sockaddrStorage};
}

} // namespace traceroute
