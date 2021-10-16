#include <Traceroute/SocketProviders/IcmpSocketProvider.hpp>
#include "utils/CreateSocket.hpp"

namespace traceroute::socketProviders
{

std::vector<Socket> IcmpSocketProvider::getSockets(const SocketAddress &addressToBind)
{
    auto socket = utils::createIcmpSocket(addressToBind);
    socket.isReceiving = true;
    socket.isSending = true;
    return {socket};
}


} // namespace traceroute::socketProviders