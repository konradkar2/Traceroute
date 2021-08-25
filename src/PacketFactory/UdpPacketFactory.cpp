#include <Traceroute/PacketFactory/UdpPacketFactory.hpp>
#include <Traceroute/Packet/UdpPacket.hpp>
namespace Traceroute
{
   UdpPacketFactory::UdpPacketFactory(const SocketAddress &source, const SocketAddress &destination, int dport)
    {
        mSource = source;
        mDestination = destination;
        mDport = dport;
    }
    std::unique_ptr<Packet> UdpPacketFactory::createPacket()
    {
        return make_unique<UdpPacket>(mSource,mDestination,mDport);
    }
}