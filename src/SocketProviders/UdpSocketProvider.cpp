#include "utils/CreateSocket.hpp"
#include <Traceroute/SocketProviders/UdpSocketProvider.hpp>

namespace traceroute::socketProviders
{

std::vector<Socket> UdpSocketProvider::getSockets(const SocketAddress &addressToBind)
{
    auto icmpSocket = utils::createIcmpSocket(addressToBind);
    icmpSocket.isReceiving = true;
    icmpSocket.isSending = false;

    auto udpSocket = utils::createUdpSocket(addressToBind);
    udpSocket.isSending = true;
    udpSocket.isReceiving = false;
    
    return {icmpSocket,udpSocket};
}

} // namespace traceroute::socketProviders