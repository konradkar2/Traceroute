#pragma once
#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/Socket.hpp>
namespace traceroute::utils
{


Socket createIcmpSocket(const SocketAddress &addressToBind);
Socket createTcpSocket(const SocketAddress &addressToBind);
Socket createUdpSocket(const SocketAddress &addressToBind);


} // namespace traceroute::utils