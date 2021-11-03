#include <Traceroute/Packet/TcpPacket.hpp>
#include <Traceroute/PacketFactory/TcpPacketFactory.hpp>
namespace traceroute
{
using namespace traceroute::packet;
TcpPacketFactory::TcpPacketFactory(const SocketAddress &source, const SocketAddress &destination, int dport)
    : mSource{source}, mDestination{destination}, mDport{dport}
{
}
std::unique_ptr<Packet> TcpPacketFactory::createPacket()
{
    return std::make_unique<TcpPacket>(mSource, mDestination, mDport);
}
} // namespace traceroute