#include "../ChecksumCalculator.hpp"
#include "../PodSerializer.hpp"
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <arpa/inet.h>
#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>
#include <stdexcept>

namespace traceroute::packet
{
std::string IcmpPacket::serialize() const
{
    return PodSerializer::serialize<IcmpHeader>(mIcmpHeader);
}
const IcmpHeader &IcmpPacket::GetIcmpHeader() const
{
    return mIcmpHeader;
}
IcmpPacket::IcmpPacket(const SocketAddress &source, const SocketAddress &destination) : Packet(source, destination)
{
}
IcmpPacket IcmpPacket::CreateIcmp4Packet(const SocketAddress &source, const SocketAddress &destination)
{
    IcmpPacket packet(source, destination);

    packet.mIcmpHeader.type = ICMP_ECHO;
    packet.mIcmpHeader.id = htons(rand() % 65536); // random id
    packet.mIcmpHeader.sequence = 0;
    packet.mIcmpHeader.code = 0;
    packet.mIcmpHeader.checksum = ChecksumCalculator::computeICMPHeaderChecksum(packet.mIcmpHeader);

    return packet;
}
IcmpPacket IcmpPacket::CreateIcmp6Packet(const SocketAddress &source, const SocketAddress &destination)
{
    IcmpPacket packet(source, destination);
    packet.mIcmpHeader.type = ICMP6_ECHO_REQUEST;
    packet.mIcmpHeader.id = htons(rand() % 65536); // random id
    packet.mIcmpHeader.sequence = 0;
    packet.mIcmpHeader.code = 0;
    packet.mIcmpHeader.checksum = 0; // leave it to kernel
    return packet;
}
} // namespace traceroute::packet