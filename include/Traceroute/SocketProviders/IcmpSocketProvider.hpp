#pragma once
#include <Traceroute/IProvideSockets.hpp>

namespace traceroute
{
namespace socketProviders

{
class IcmpSocketProvider : public IProvideSockets
{
  public:
    std::vector<SocketExt> getSockets(const SocketAddress &addressToBind) override;
};
} // namespace socketProviders
} // namespace traceroute