#pragma once
#include <Traceroute/SocketAddress.hpp>
#include <optional>
#include <string>

namespace traceroute {

/// Resolves source address which is going to be assigned to an outgoing packet,
/// It is neccesery to calculate TCP/UDP checksum,
/// because for Ipv4 raw socket the kernel doesn't calculate it
SocketAddress resolveSourceAddress(const SocketAddress &destination, const std::optional<std::string> & ifaceName);

} // namespace traceroute
