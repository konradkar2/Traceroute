#include <Traceroute/Packet.hpp>
#include <stdexcept>
#include "PodSerializer.hpp"
#include "ChecksumCalculator.hpp"
namespace Traceroute
{

    Packet::Packet(const SocketAddress &source,
                   const SocketAddress &destination) : mSourceAddress(source), mDestinationAddress(destination)
    {
        if (destination.getFamily() != source.getFamily())
        {
            throw runtime_error("source address family doesnt match destination");
        }
    }
    const SocketAddress &Packet::getSourceAddress() const
    {
        return mSourceAddress;
    }
    const SocketAddress &Packet::getDestinationAddress() const
    {
        return mDestinationAddress;
    }
    int Packet::getFamily() const
    {
        return mSourceAddress.getFamily();
    }

    std::string TcpPacket::serialize() const
    {
        return PodSerializer<TcpHeader>::serialize(mTcpHeader);
    }

    std::string UdpPacket::serialize() const
    {
        return PodSerializer<UdpHeader>::serialize(mUdpHeader);
    }

    std::string IcmpPacket::serialize() const
    {
        return PodSerializer<IcmpHeader>::serialize(mIcmpHeader);
    }
    const IcmpHeader &IcmpPacket::GetIcmpHeader() const
    {
        return mIcmpHeader;
    }
    const TcpHeader &TcpPacket::getTcpHeader() const
    {
        return mTcpHeader;
    }
    const UdpHeader &UdpPacket::GetUdpHeader() const
    {
        return mUdpHeader;
    }
}
