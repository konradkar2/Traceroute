#include "utils/CreateSocket.hpp"
#include <Traceroute/SocketProviders/TcpSocketProvider.hpp>

namespace traceroute::socketProviders {

std::vector<SocketExt> TcpSocketProvider::createSockets(const SocketAddress &addressToBind,
                                                        std::optional<std::string> ifaceNameToBind)
{
    SocketExt icmpSe;
    icmpSe.socket = utils::createIcmpRawSocket(addressToBind, ifaceNameToBind);
    icmpSe.role = Role::Receive;

    SocketExt tcpSe;
    tcpSe.socket = utils::createTcpRawSocket(addressToBind, ifaceNameToBind);
    tcpSe.role = Role::Receive | Role::Send;

    return {icmpSe, tcpSe};
}

} // namespace traceroute::socketProviders