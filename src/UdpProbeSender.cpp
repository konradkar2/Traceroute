#include <Traceroute/UdpProbeSender.hpp>



namespace Traceroute
{
    UdpProbeSender::UdpProbeSender(IDataSender * dataSender)
        : ProbeSenderBase(dataSender)
    {
               
    }    
    ProbeResultContainer UdpProbeSender::SendProbe(
            const UdpPacket * packet, int ttl,const int retries, const int timeoutms )
    {
        _packet = packet;       
        return BeginProbing(packet, ttl,retries,timeoutms);
    }    
    
    bool UdpProbeSender::IsResponseValid(const SocketAddress & client, const int & protocol) 
    {
           
        bool isResponseValid = false;
        char * ptr = GetReceiveBuf();
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
                _packet->getDestinationAddress()
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
                _packet->getDestinationAddress()
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

