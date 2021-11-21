#include "TcpToTcpResponseValidator.hpp"
#include "ResponseValidators/common.hpp"
#include "Traceroute/Packet/TcpPacket.hpp"
#include "common.hpp"
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

namespace traceroute::responseValidators {
using namespace traceroute::packet;

TcpToTcpResponseValidator::TcpToTcpResponseValidator(const TcpPacket &tcpPacket) : mTcpPacket(tcpPacket)
{
}

bool TcpToTcpResponseValidator::validateFields(const ResponseInfo &responseInfo, const char *response)
{
    if (mTcpPacket.getDestinationAddress() == responseInfo.client())
    {
        const TcpHeader *tcp_hdr = reinterpret_cast<const TcpHeader *>(response);
        unsigned int     seq     = ntohl(mTcpPacket.getTcpHeader().seq);
        unsigned int     ack_seq = ntohl(tcp_hdr->ack_seq);
        if ((ack_seq - 1) == seq)
            return true;
    }
    return false;
}

bool TcpToTcpResponseValidator::validateSize(size_t size)
{
    return true;
}

bool TcpToTcpResponseValidator::validateProtocol(int protocol)
{
    return protocol == IPPROTO_TCP;
}

} // namespace traceroute::responseValidators
