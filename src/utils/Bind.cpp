#include "Bind.hpp"
#include <stdexcept>
#include <string>
#include <sys/socket.h>

namespace traceroute::utils {

void bindInterface(int sfd, const std::string &ifaceName)
{
    if (setsockopt(sfd, SOL_SOCKET, SO_BINDTODEVICE, ifaceName.c_str(), ifaceName.size()) < 0)
    {
        throw std::runtime_error("Could not bind to an interface: " + ifaceName);
    }
}

void bindIpAddress(int sfd, const SocketAddress &address)
{
    if (bind(sfd, address.sockaddrP(), address.size()) < 0)
    {
        throw std::runtime_error("Could not bind address: " + address.toString());
    }
}

} // namespace traceroute::utils
