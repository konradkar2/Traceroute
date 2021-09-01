#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
namespace traceroute::responseValidators
{

    bool TcpResponseValidator::isResponseValid(const Packet &request, const SocketAddress &client,
                                               int protocol, const char *response, size_t responseSize)
    {
        const auto &tcpPacket = dynamic_cast<const packet::TcpPacket &>(request);
        bool isResponseValid = false;
        const char *pResponse = response;
        int family = client.family();

        //skip ipv4 header
        if (family == AF_INET)
        {
            size_t iphdr_size = 0;
            const Ipv4Header *ip_hdr = reinterpret_cast<const Ipv4Header *>(pResponse);
            iphdr_size = ip_hdr->ihl << 2;
            pResponse += iphdr_size;
        }
        switch (protocol)
        {
        case IPPROTO_ICMP:
        {
            const IcmpHeader *icmp_hdr = reinterpret_cast<const IcmpHeader *>(pResponse);
            //we hit transit router
            //or we hit target but its port/protocol is blocked
            if (
                (icmp_hdr->type == ICMP_TIME_EXCEEDED ||
                 icmp_hdr->type == ICMP_UNREACH_PORT ||
                 icmp_hdr->type == ICMP_UNREACH_PROTOCOL) &&
                tcpPacket.getDestinationAddress() == client)
            {
                pResponse += sizeof(IcmpHeader);
                pResponse += sizeof(Ipv4Header);
                const TcpHeader *inner_tcp_hdr = reinterpret_cast<const TcpHeader *>(pResponse);
                if (inner_tcp_hdr->seq == tcpPacket.getTcpHeader().seq)
                {
                    isResponseValid = true;
                }
            }
            break;
        }
        case IPPROTO_ICMPV6:
        {
            const IcmpHeader *header = reinterpret_cast<const IcmpHeader *>(pResponse);
            if (header->type == ICMP6_TIME_EXCEEDED ||
                (header->type == ICMP6_DST_UNREACH &&
                 tcpPacket.getDestinationAddress() == client))
            {
                pResponse += sizeof(IcmpHeader);
                pResponse += Ipv6HeaderSize;
                const TcpHeader *inner_tcp_hdr = reinterpret_cast<const TcpHeader *>(pResponse);
                if (inner_tcp_hdr->seq == tcpPacket.getTcpHeader().seq)
                {
                    isResponseValid = true;
                }
            }
            break;
        }
        case IPPROTO_TCP:
        {
            if (tcpPacket.getDestinationAddress() == client)
            {
                const TcpHeader *tcp_hdr = reinterpret_cast<const TcpHeader *>(pResponse);
                unsigned int seq = ntohl(tcpPacket.getTcpHeader().seq);
                unsigned int ack_seq = ntohl(tcp_hdr->ack_seq);
                if ((ack_seq - 1) == seq)
                    isResponseValid = true;
            }
            break;
        }
        }

        return isResponseValid;
    }

}
