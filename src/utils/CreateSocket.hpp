#pragma once

#include <Traceroute/Socket.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <optional>

namespace traceroute::utils {

Socket setupIcmpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName);
Socket setupTcpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName);
Socket setupUdpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName);

} // namespace traceroute::utils