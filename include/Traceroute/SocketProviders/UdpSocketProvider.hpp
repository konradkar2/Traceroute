#pragma once
#include <Traceroute/IProvideSockets.hpp>

namespace traceroute
{
namespace socketProviders

{
class UdpSocketProvider : public IProvideSockets
{
  public:
    std::vector<SocketExt> getSockets(const traceroute::SocketAddress &addressToBind) override;
};
} // namespace socketProviders
} // namespace traceroute