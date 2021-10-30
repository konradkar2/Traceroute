#include "utils/CreateSocket.hpp"
#include <Traceroute/SocketProviders/UdpSocketProvider.hpp>

namespace traceroute::socketProviders
{

std::vector<SocketExt> UdpSocketProvider::getSockets(const SocketAddress &addressToBind)
{
    SocketExt icmpSe;
    icmpSe.socket = utils::createIcmpRawSocket(addressToBind);
    icmpSe.role = Role::Receive;

    SocketExt udpSe;
    udpSe.socket = utils::createUdpRawSocket(addressToBind);
    udpSe.role = Role::Send;
    return {icmpSe, udpSe};
}

} // namespace traceroute::socketProviders