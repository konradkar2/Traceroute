#include "utils/CreateSocket.hpp"
#include <Traceroute/SocketProviders/TcpSocketProvider.hpp>

namespace traceroute::socketProviders
{

std::vector<Socket> TcpSocketProvider::getSockets(const SocketAddress &addressToBind)
{
    auto icmpSocket = utils::createIcmpSocket(addressToBind);
    icmpSocket.isReceiving = true;
    icmpSocket.isSending = false;

    auto tcpSocket = utils::createTcpSocket(addressToBind);
    tcpSocket.isSending = true;
    tcpSocket.isReceiving = true;
    
    return {icmpSocket,tcpSocket};
}

} // namespace traceroute::socketProviders