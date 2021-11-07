#pragma once
#include <Traceroute/IProvideSockets.hpp>

namespace traceroute {
namespace socketProviders

{
class TcpSocketProvider : public IProvideSockets
{
  public:
    std::vector<SocketExt> createSockets(const SocketAddress &addressToBind,
                                         std::optional<std::string> ifaceNameToBind) override;
};
} // namespace socketProviders
} // namespace traceroute