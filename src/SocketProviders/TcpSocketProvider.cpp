#include "utils/CreateSocket.hpp"
#include <Traceroute/SocketProviders/TcpSocketProvider.hpp>

namespace traceroute::socketProviders
{

std::vector<SocketExt> TcpSocketProvider::getSockets(const SocketAddress &addressToBind)
{
    SocketExt icmpSe;
    icmpSe.socket = utils::createIcmpRawSocket(addressToBind);
    icmpSe.role = Role::Receive;

    SocketExt tcpSe;
    tcpSe.socket = utils::createTcpRawSocket(addressToBind);
    tcpSe.role = Role::Receive | Role::Send;

    return {icmpSe, tcpSe};
}

} // namespace traceroute::socketProviders