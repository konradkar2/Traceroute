#include "utils/CreateSocket.hpp"
#include <Traceroute/SocketProviders/UdpSocketProvider.hpp>

namespace traceroute::socketProviders {

std::vector<SocketExt> UdpSocketProvider::createSockets(const SocketAddress &addressToBind,
                                                        std::optional<std::string> ifaceNameToBind)
{
    SocketExt icmpSe;
    icmpSe.socket = utils::createIcmpRawSocket(addressToBind, ifaceNameToBind);
    icmpSe.role = Role::Receive;

    SocketExt udpSe;
    udpSe.socket = utils::createUdpRawSocket(addressToBind, ifaceNameToBind);
    udpSe.role = Role::Send;
    return {icmpSe, udpSe};
}

} // namespace traceroute::socketProviders