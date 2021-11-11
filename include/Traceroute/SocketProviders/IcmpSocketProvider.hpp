#pragma once
#include <Traceroute/interface/IProvideSockets.hpp>

namespace traceroute {
namespace socketProviders

{
class IcmpSocketProvider : public IProvideSockets
{
  public:
    std::vector<SocketExt> createSockets(const SocketAddress &addressToBind,
                                         std::optional<std::string> ifaceNameToBind) override;
};
} // namespace socketProviders
} // namespace traceroute