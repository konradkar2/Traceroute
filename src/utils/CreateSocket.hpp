#pragma once
#include <Traceroute/Socket.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <optional>
namespace traceroute::utils {

Socket createIcmpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName);
Socket createTcpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName);
Socket createUdpRawSocket(const SocketAddress &addressToBind, std::optional<std::string> ifaceName);

} // namespace traceroute::utils