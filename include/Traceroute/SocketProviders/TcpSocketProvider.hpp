#pragma once
#include <Traceroute/IProvideSocket.hpp>

namespace traceroute
{
namespace socketProviders

{
class TcpSocketProvider : public IProvideSocket
{
  public:
    std::vector<Socket> getSockets(const SocketAddress &addressToBind) override;
};
} // namespace socketProviders
} // namespace traceroute