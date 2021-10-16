#pragma once
#include <Traceroute/IProvideSocket.hpp>

namespace traceroute
{
namespace socketProviders

{
class IcmpSocketProvider : public IProvideSocket
{
  public:
    std::vector<Socket> getSockets(const SocketAddress &addressToBind) override;
};
} // namespace socketProviders
} // namespace traceroute