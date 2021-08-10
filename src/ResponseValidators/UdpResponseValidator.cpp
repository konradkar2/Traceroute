#include <Traceroute/ResponseValidators/UdpResponseValidator.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>

namespace Traceroute
{
    namespace ResponseValidators
    {
        bool UdpResponseValidator::isResponseValid(const Packet &request, const SocketAddress &client,
                                                   int protocol, const char *response, size_t responseSize)
        {
            const auto & udpPacket = dynamic_cast<const UdpPacket &>(request);
            bool isResponseValid = false;
            const char *pResponse = response;
            const int family = client.getFamily();

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
                const IcmpHeader *header = reinterpret_cast<const IcmpHeader *>(pResponse);
                //we hit transit router
                //or we hit target with blocked port
                if (
                    (header->type == ICMP_TIME_EXCEEDED ||
                     header->type == ICMP_UNREACH_PORT ||
                     header->type == ICMP_UNREACH_PROTOCOL) &&
                    udpPacket.getDestinationAddress() == client) //we hit target
                {
                    //cant really check anything, lets assume its proper
                    isResponseValid = true;
                }
                break;
            }
            case IPPROTO_ICMPV6:
            {
                const IcmpHeader *header = reinterpret_cast<const IcmpHeader *>(pResponse);
                if (header->type == ICMP6_TIME_EXCEEDED ||
                    (header->type == ICMP6_DST_UNREACH &&
                     udpPacket.getDestinationAddress() == client))
                {
                    isResponseValid = true;
                }
                break;
            }
            }

            return isResponseValid;
        }

    }

}