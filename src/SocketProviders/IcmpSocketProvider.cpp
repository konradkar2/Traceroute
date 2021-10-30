#include "utils/CreateSocket.hpp"
#include <Traceroute/SocketProviders/IcmpSocketProvider.hpp>

namespace traceroute::socketProviders
{

std::vector<SocketExt> IcmpSocketProvider::getSockets(const SocketAddress &addressToBind)
{
    SocketExt icmpSe;
    icmpSe.socket = utils::createIcmpRawSocket(addressToBind);
    icmpSe.role = Role::Receive | Role::Send;
    fprintf(stderr,"%d\n",icmpSe.role);
    return {icmpSe};
}

} // namespace traceroute::socketProviders