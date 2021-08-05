#include <Traceroute/IcmpProbeSender.hpp>

namespace Traceroute
{
    
    IcmpProbeSender::IcmpProbeSender(IDataSender * dataSender)
        : ProbeSenderBase(dataSender)
    {
        
    }
    ProbeResultContainer IcmpProbeSender::SendProbe(
            const IcmpPacket * packet,int ttl, const int retries, const int timeoutms )
    {
        _packet = packet;
        return BeginProbing(packet,ttl,retries,timeoutms);           
    }    
    bool IcmpProbeSender::IsResponseValid(const SocketAddress & client, const int & protocol) 
    {
        bool isResponseValid = false;
        char * ptr = GetReceiveBuf();       
        int family  = client.GetFamily();
        
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
                const IcmpHeader * icmp_hdr =  reinterpret_cast<const IcmpHeader *>(ptr);
                //we propably hit the target
                if(icmp_hdr->type == ICMP_ECHOREPLY) 
                {
                    if(client.IsSameAs(_packet->GetDestinationAddress()) && 
                    (icmp_hdr->sequence == _packet->GetIcmpHeader().sequence))
                    {
                        isResponseValid = true;
                    }
                }
                //we hit transit router
                else if(icmp_hdr->type == ICMP_TIME_EXCEEDED) //we hit transit router
                {                      
                    ptr += sizeof(IcmpHeader); 
                    ptr += sizeof(Ipv4Header); //skip ip header
                    //look inside inner icmp header and check if it belongs to us
                    const IcmpHeader * inner_icmp_hdr =  reinterpret_cast<const IcmpHeader *>(ptr);
                    if(inner_icmp_hdr->id == _packet->GetIcmpHeader().id)
                    {
                        isResponseValid = true;
                    }
                } 
                break;
            }
            case IPPROTO_ICMPV6:
            {
                const IcmpHeader * header = reinterpret_cast<const IcmpHeader *>(ptr);
                //we hit transit router
                if(header->type == ICMP6_TIME_EXCEEDED)
                {
                    ptr += sizeof(IcmpHeader);
                    ptr += Ipv6HeaderSize; //skip ipv6 inner header
                    const IcmpHeader * inner_icmp_header = reinterpret_cast<const IcmpHeader *>(ptr);
                    if(inner_icmp_header->id == _packet->GetIcmpHeader().id)
                    {
                        isResponseValid = true;
                    }
                }
                else if((header->type == ICMP6_ECHO_REPLY) &&
                    header->sequence == _packet->GetIcmpHeader().sequence && //check sequence
                    _packet->GetDestinationAddress().IsSameAs(client) //check ip    
                    //check sequence
                )
                {
                    isResponseValid = true;
                }
                break;
            }
        }

              
        
        return isResponseValid;
    }

   
    
}

