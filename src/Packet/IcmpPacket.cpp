#include "utils/ChecksumCalculator.hpp"
#include "utils/Serialize.hpp"
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include <arpa/inet.h>
#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>
#include <stdexcept>

namespace traceroute::packet {
std::string IcmpPacket::serialize() const
{
    return utils::serialize<IcmpHeader>(mIcmpHeader);
}

const IcmpHeader &IcmpPacket::GetIcmpHeader() const
{
    return mIcmpHeader;
}

IcmpPacket::IcmpPacket(const SocketAddress &source, const SocketAddress &destination) : Packet(source, destination)
{
}

std::unique_ptr<IcmpPacket> IcmpPacket::CreateIcmp4Packet(const SocketAddress &source, const SocketAddress &destination)
{
    std::unique_ptr<IcmpPacket> packet{new IcmpPacket{source, destination}};

    packet->mIcmpHeader.id       = htons(rand() % 65536); // random id
    packet->mIcmpHeader.type     = ICMP_ECHO;
    packet->mIcmpHeader.sequence = 0;
    packet->mIcmpHeader.code     = 0;
    packet->mIcmpHeader.checksum = utils::ChecksumCalculator::computeICMPHeaderChecksum(packet->mIcmpHeader);
    return packet;
}

std::unique_ptr<IcmpPacket> IcmpPacket::CreateIcmp6Packet(const SocketAddress &source, const SocketAddress &destination)
{
    std::unique_ptr<IcmpPacket> packet{new IcmpPacket{source, destination}};
    packet->mIcmpHeader.type     = ICMP6_ECHO_REQUEST;
    packet->mIcmpHeader.id       = htons(rand() % 65536); // random id
    packet->mIcmpHeader.sequence = 0;
    packet->mIcmpHeader.code     = 0;
    packet->mIcmpHeader.checksum = 0; // leave it to kernel
    return packet;
}

bool IcmpPacket::isValid(const ResponseInfo &responseInfo, const char *response)
{
    responseValidators::IcmpResponseValidator validator(*this);
    return validator.isValid(responseInfo, response);
}

} // namespace traceroute::packet