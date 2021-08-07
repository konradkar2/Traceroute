#include "IcmpResponseValidator.hpp"
#include "Packet.hpp"
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>
#include <sys/socket.h>

namespace Traceroute
{
    namespace Icmp
    {
        bool IcmpResponseValidator::isResponseValid(const Packet &request, const SocketAddress &client,
                                                    int protocol, const char *response, size_t responseSize)
        {
            const auto &icmpPacket = dynamic_cast<const IcmpPacket &>(request);

            const char *pResponse = response;

            bool isResponseValid = false;
            int family = client.getFamily();

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
                //we propably hit the target
                if (icmp_hdr->type == ICMP_ECHOREPLY)
                {
                    if (client == icmpPacket.getDestinationAddress() &&
                        (icmp_hdr->sequence == icmpPacket.GetIcmpHeader().sequence))
                    {
                        isResponseValid = true;
                    }
                }
                //we hit transit router
                else if (icmp_hdr->type == ICMP_TIME_EXCEEDED) //we hit transit router
                {
                    pResponse += sizeof(IcmpHeader);
                    pResponse += sizeof(Ipv4Header); //skip ip header
                    //look inside inner icmp header and check if it belongs to us
                    const IcmpHeader *inner_icmp_hdr = reinterpret_cast<const IcmpHeader *>(pResponse);
                    if (inner_icmp_hdr->id == icmpPacket.GetIcmpHeader().id)
                    {
                        isResponseValid = true;
                    }
                }
                break;
            }
            case IPPROTO_ICMPV6:
            {
                const IcmpHeader *header = reinterpret_cast<const IcmpHeader *>(pResponse);
                //we hit transit router
                if (header->type == ICMP6_TIME_EXCEEDED)
                {
                    pResponse += sizeof(IcmpHeader);
                    pResponse += Ipv6HeaderSize; //skip ipv6 inner header
                    const IcmpHeader *inner_icmp_header = reinterpret_cast<const IcmpHeader *>(pResponse);
                    if (inner_icmp_header->id == icmpPacket.GetIcmpHeader().id)
                    {
                        isResponseValid = true;
                    }
                }
                else if ((header->type == ICMP6_ECHO_REPLY) &&
                         header->sequence == icmpPacket.GetIcmpHeader().sequence &&
                         icmpPacket.getDestinationAddress() == client)
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
