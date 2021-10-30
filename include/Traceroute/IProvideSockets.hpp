#pragma once

#include "Socket.hpp"
#include "SocketAddress.hpp"
#include <vector>
namespace traceroute
{

class IProvideSockets
{
  public:
    virtual std::vector<SocketExt> getSockets(const traceroute::SocketAddress &addressToBind) = 0;
    virtual ~IProvideSockets() = default;
};
} // namespace traceroute