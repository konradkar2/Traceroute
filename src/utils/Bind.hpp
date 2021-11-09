#pragma once
#include <Traceroute/SocketAddress.hpp>
#include <string>

namespace traceroute::utils {

void bindInterface(int sfd, const std::string &ifaceName);
void bindIpAddress(int sfd, const SocketAddress &address);

} // namespace traceroute::utils
