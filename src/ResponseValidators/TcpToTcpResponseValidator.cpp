#include "TcpToTcpResponseValidator.hpp"
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

bool TcpToTcpResponseValidator::validateFields(const Packet &request, const SocketAddress &client, const char *response,
                                               size_t responseSize)
{
    const auto &tcpPacket = dynamic_cast<const TcpPacket &>(request);
    if (tcpPacket.getDestinationAddress() == client)
    {
        const TcpHeader *tcp_hdr = reinterpret_cast<const TcpHeader *>(response);
        unsigned int seq = ntohl(tcpPacket.getTcpHeader().seq);
        unsigned int ack_seq = ntohl(tcp_hdr->ack_seq);
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
