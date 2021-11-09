#include "utils/CreateSocket.hpp"
#include <Traceroute/SocketProviders/IcmpSocketProvider.hpp>

namespace traceroute::socketProviders {

std::vector<SocketExt> IcmpSocketProvider::createSockets(const SocketAddress &addressToBind,
                                                         std::optional<std::string> ifaceNameToBind)
{
    SocketExt icmpSe;
    icmpSe.socket = utils::setupIcmpRawSocket(addressToBind, ifaceNameToBind);
    icmpSe.role = Role::Receive | Role::Send;
    return {icmpSe};
}

} // namespace traceroute::socketProviders