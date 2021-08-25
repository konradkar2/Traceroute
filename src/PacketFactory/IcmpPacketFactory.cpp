#include <Traceroute/PacketFactory/IcmpPacketFactory.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
namespace Traceroute
{
    IcmpPacketFactory::IcmpPacketFactory(const SocketAddress &source, const SocketAddress &destination)
    {
        mSource = source;
        mDestination = destination;
    }
    std::unique_ptr<Packet> IcmpPacketFactory::createPacket()
    {
        if(mSource.isV4())
            return make_unique<IcmpPacket>(IcmpPacket::CreateIcmp4Packet(mSource,mDestination));
        else
            return make_unique<IcmpPacket>(IcmpPacket::CreateIcmp6Packet(mSource,mDestination));
    }

  
}