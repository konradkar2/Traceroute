#pragma once
#include <Traceroute/Socket.hpp>
#include <Traceroute/SocketAddress.hpp>
namespace traceroute::utils
{

Socket createIcmpRawSocket(const SocketAddress &addressToBind);
Socket createTcpRawSocket(const SocketAddress &addressToBind);
Socket createUdpRawSocket(const SocketAddress &addressToBind);

} // namespace traceroute::utils