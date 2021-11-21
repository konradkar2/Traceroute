#include "Traceroute/HeaderTypes.hpp"
#include "Traceroute/ResponseInfo.hpp"
#include "common.hpp"
#include <Traceroute/Packet/UdpPacket.hpp>
#include <Traceroute/ResponseValidators/UdpResponseValidator.hpp>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

namespace traceroute::responseValidators {

UdpResponseValidator::UdpResponseValidator(const packet::UdpPacket &udpPacket) : mUdpPacket(udpPacket)
{
}
bool UdpResponseValidator::validateFields(const ResponseInfo &responseInfo, const char *response)
{
    // for now we can't check anything due to UDP header simplicity
    // TODO: send additional data with UDP header in order to validate TIME_EXCEEDED and DST_UNREACH
    const auto       &client     = responseInfo.client();
    const IcmpHeader *header     = reinterpret_cast<const IcmpHeader *>(response);
    const auto        headerType = header->type;
    if (mUdpPacket.getDestinationAddress() == client)
    {
        return headerType == ICMP_DEST_UNREACH or headerType == ICMP6_DST_UNREACH;
    }
    else if (headerType == ICMP_TIME_EXCEEDED or headerType == ICMP6_TIME_EXCEEDED)
    {
        return true;
    }
    return false;
}
bool UdpResponseValidator::validateProtocol(int protocol)
{
    return protocol == IPPROTO_ICMP or protocol == IPPROTO_ICMPV6;
}
bool UdpResponseValidator::validateSize(size_t size)
{
    return size >= sizeof(IcmpHeader);
}

} // namespace traceroute::responseValidators
