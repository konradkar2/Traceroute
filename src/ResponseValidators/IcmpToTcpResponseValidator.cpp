#include "IcmpToTcpResponseValidator.hpp"
#include "ResponseValidators/common.hpp"
#include "Traceroute/Packet/TcpPacket.hpp"
#include "common.hpp"
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

namespace traceroute::responseValidators
{
using namespace traceroute::packet;
namespace
{
const char *advanceToTcpHeader(const char *icmpHeader)
{
    icmpHeader += sizeof(IcmpHeader);
    const char *tcpHeader = icmpHeader + getIpHeaderSize(icmpHeader);
    return tcpHeader;
}

bool validateDestUnreach(const char *icmpHeader, const TcpPacket &tcpPacket, const SocketAddress &client)
{
    if (client == tcpPacket.getDestinationAddress())
    {
        const char *tcpHeader = advanceToTcpHeader(icmpHeader);
        return validateSameSequence(tcpHeader, tcpPacket.getTcpHeader());
    }
    return false;
}
bool validateTimeExceeded(const char *icmpHeader, const TcpPacket &tcpPacket)
{
    const char *tcpHeader = advanceToTcpHeader(icmpHeader);
    return validateSameSequence(tcpHeader, tcpPacket.getTcpHeader());
}

} // namespace

bool IcmpToTcpResponseValidator::validateFields(const Packet &request, const SocketAddress &client,
                                                const char *response, size_t responseSize)
{
    const auto &tpcPacket = dynamic_cast<const TcpPacket &>(request);

    if (client.isV4())
    {
        switch (getIcmpType(response))
        {
        case ICMP_DEST_UNREACH:
            return validateDestUnreach(response, tpcPacket, client);
        case ICMP_TIME_EXCEEDED:
            return validateTimeExceeded(response, tpcPacket);
        }
    }
    else
    {
        switch (getIcmpType(response))
        {
        case ICMP6_DST_UNREACH:
            return validateDestUnreach(response, tpcPacket, client);
        case ICMP6_TIME_EXCEEDED:
            return validateTimeExceeded(response, tpcPacket);
        }
    }
    return false;
}

bool IcmpToTcpResponseValidator::validateSize(size_t size)
{
    return true;
}

bool IcmpToTcpResponseValidator::validateProtocol(int protocol)
{
    return protocol == IPPROTO_ICMP || protocol == IPPROTO_ICMPV6;
}

} // namespace traceroute::responseValidators
