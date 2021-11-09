#pragma once
#include <Traceroute/SocketAddress.hpp>
#include <optional>
#include <string>

namespace traceroute::utils {

/// Resolves source address which is going to be assigned to an outgoing packet
/// It is neccesery to get it because for Ipv4 raw socket the kernel doesn't calculate checksum for tcp/udp
SocketAddress resolveSourceAddress(const SocketAddress &destination, std::optional<std::string> ifaceName);

} // namespace traceroute::utils
