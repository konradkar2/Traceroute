#pragma once

#include "Socket.hpp"
#include "SocketAddress.hpp"
#include <vector>
namespace traceroute
{

class IProvideSocket
{
  public:
    virtual std::vector<Socket> getSockets(const traceroute::SocketAddress &addressToBind) = 0;
    virtual ~IProvideSocket() = default;
};
} // namespace traceroute