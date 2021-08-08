#include "Icmp4ResponseValidator.hpp"
#include <netinet/ip_icmp.h>
#include <cassert>
namespace Traceroute
{
    namespace Icmp
    {
        namespace V4
        {
            bool Icmp4ResponseValidator::isResponseValid(const Packet &request, const SocketAddress &client,
                                                         int protocol, const char *response, size_t responseSize)
            {
                bool isResponseValid = false;
                const auto &icmpPacket = dynamic_cast<const IcmpPacket &>(request);
                const char *pResponse = response;

                //skip ipv4 header
                size_t iphdr_size = 0;
                const Ipv4Header *ip_hdr = reinterpret_cast<const Ipv4Header *>(pResponse);
                iphdr_size = ip_hdr->ihl << 2;
                pResponse += iphdr_size;

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
                    const Ipv4Header * icmpInnerIpHeader = reinterpret_cast<const Ipv4Header *>(pResponse);
                    size_t icmpInnerIpHeaderSize = icmpInnerIpHeader->ihl << 2;
                    pResponse += icmpInnerIpHeaderSize; //skip ip header
                    //look inside inner icmp header and check if it belongs to us
                    const IcmpHeader *inner_icmp_hdr = reinterpret_cast<const IcmpHeader *>(pResponse);
                    if (inner_icmp_hdr->id == icmpPacket.GetIcmpHeader().id)
                    {
                        isResponseValid = true;
                    }
                }
                return isResponseValid;
            }
            
        }
    }
}
