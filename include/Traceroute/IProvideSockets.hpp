#pragma once

#include "Socket.hpp"
#include "SocketAddress.hpp"
#include <optional>
#include <vector>
namespace traceroute {

class IProvideSockets
{
  public:
    virtual std::vector<SocketExt> createSockets(const SocketAddress &addressToBind,
                                                 std::optional<std::string> ifaceNameToBind) = 0;
    virtual ~IProvideSockets() = default;
};
} // namespace traceroute