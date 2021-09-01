#include <stdexcept>
#include "../PodSerializer.hpp"
#include "../ChecksumCalculator.hpp"
#include <Traceroute/Packet/UdpPacket.hpp>
#include <arpa/inet.h>

namespace traceroute::packet
{
    std::string UdpPacket::serialize() const
    {
        return PodSerializer::serialize<UdpHeader>(mUdpHeader);
    }
    const UdpHeader &UdpPacket::GetUdpHeader() const
    {
        return mUdpHeader;
    }
    UdpPacket::UdpPacket(const SocketAddress &source,
                         const SocketAddress &destination, int dport)
        : Packet(source, destination)
    {
        mUdpHeader.source = htons(50000 + rand() % 15001);
        mUdpHeader.dest = htons(dport);
        mUdpHeader.len = htons(sizeof(mUdpHeader));
        mUdpHeader.check = 0;
        if (source.isV4())
        {
            mUdpHeader.check = ChecksumCalculator::computeUdpHeaderChecksum(mUdpHeader, source, destination);
        }
    }
}