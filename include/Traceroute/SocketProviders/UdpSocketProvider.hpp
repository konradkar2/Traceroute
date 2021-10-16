#pragma once
#include <Traceroute/IProvideSocket.hpp>

namespace traceroute
{
namespace socketProviders

{
class UdpSocketProvider : public IProvideSocket
{
  public:
    std::vector<Socket> getSockets(const traceroute::SocketAddress &addressToBind) override;
};
} // namespace socketProviders
} // namespace traceroute