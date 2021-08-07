#include <Traceroute/UdpProbeSender.hpp>
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>


namespace Traceroute
{
    UdpProbeSender::UdpProbeSender(IDataSender * dataSender)
        : ProbeSenderBase(dataSender)
    {
               
    }    
    ProbeResultContainer UdpProbeSender::SendProbe(
            const UdpPacket * packet, int ttl,const int retries, std::chrono::microseconds timeout)
    {
        mPacket = packet;       
        return BeginProbing(packet, ttl,retries,timeout);
    }    
    
    bool UdpProbeSender::isResponseValid(const SocketAddress & client, const int & protocol) 
    {
           
        bool isResponseValid = false;
        char * ptr = getReceiveBuf();
        const int family = client.getFamily();

        //skip ipv4 header
        if(family == AF_INET)
        { 
            size_t iphdr_size = 0;        
            const Ipv4Header * ip_hdr = reinterpret_cast<const Ipv4Header *>(ptr);
            iphdr_size = ip_hdr->ihl << 2;
            ptr += iphdr_size;
        }       
        switch(protocol)
        {
            case IPPROTO_ICMP:
            {
                const IcmpHeader * header = reinterpret_cast<const IcmpHeader *>(ptr);                
                //we hit transit router
                //or we hit target with blocked port               
                if(
                header->type == ICMP_TIME_EXCEEDED ||
                (header->type == ICMP_UNREACH_PORT ||
                header->type == ICMP_UNREACH_PROTOCOL) &&
                mPacket->getDestinationAddress()
                        == client)//we hit target
                {                                                   
                    //cant really check anything, lets assume its proper
                    isResponseValid = true;
                }
                break;
            }
            case IPPROTO_ICMPV6:
            {
                const IcmpHeader * header = reinterpret_cast<const IcmpHeader *>(ptr);           
                if(header->type == ICMP6_TIME_EXCEEDED ||
                (header->type == ICMP6_DST_UNREACH && 
                mPacket->getDestinationAddress()
                         == client))
                {
                    isResponseValid = true;
                }
                break;
            }
        }       

        return isResponseValid;
    }
    
    
}

