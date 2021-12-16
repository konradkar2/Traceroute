#include "utils/ChecksumCalculator.hpp"
#include "utils/Serialize.hpp"
#include <Traceroute/Packet/UdpPacket.hpp>
#include <Traceroute/ResponseValidators/UdpResponseValidator.hpp>
#include <arpa/inet.h>
#include <stdexcept>

namespace traceroute::packet {
std::string UdpPacket::serialize() const
{
    return utils::serialize<UdpHeader>(mUdpHeader);
}
const UdpHeader &UdpPacket::GetUdpHeader() const
{
    return mUdpHeader;
}
UdpPacket::UdpPacket(const SocketAddress &source, const SocketAddress &destination, int dport)
    : Packet(source, destination)
{
    mUdpHeader.source = htons(50000 + rand() % 15001);
    mUdpHeader.dest   = htons(dport);
    mUdpHeader.len    = htons(sizeof(mUdpHeader));
    mUdpHeader.check  = 0;
    if (source.isV4())
    {
        mUdpHeader.check = utils::ChecksumCalculator::computeUdpHeaderChecksum(mUdpHeader, source, destination);
    }
}
bool UdpPacket::isValid(const ResponseInfo &responseInfo, const char *response)
{
    responseValidators::UdpResponseValidator validator(*this);
    return validator.isValid(responseInfo, response);
}

} // namespace traceroute::packet