#include <Traceroute/PacketFactory/TcpPacketFactory.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
namespace Traceroute
{
   TcpPacketFactory::TcpPacketFactory(const SocketAddress &source, const SocketAddress &destination, int dport)
    {
        mSource = source;
        mDestination = destination;
        mDport = dport;
    }
    std::unique_ptr<Packet> TcpPacketFactory::createPacket()
    {
        return make_unique<TcpPacket>(mSource,mDestination,mDport);
    }
}