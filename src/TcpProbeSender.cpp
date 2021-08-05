#include <Traceroute/TcpProbeSender.hpp>



namespace Traceroute
{
    TcpProbeSender::TcpProbeSender(IDataSender * dataSender)
        : ProbeSenderBase(dataSender)
    {
       
        
    }    
    ProbeResultContainer TcpProbeSender::SendProbe(
            const TcpPacket * packet, int ttl, const int retries, const int timeoutms )
    {
        _packet = packet;       
        return BeginProbing(packet,ttl,retries,timeoutms);
    }    
   
    bool TcpProbeSender::IsResponseValid(const SocketAddress & client, const int & protocol) 
    {
        bool isResponseValid = false;
        char * ptr = GetReceiveBuf();
        int family = client.GetFamily();

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
                const IcmpHeader * icmp_hdr = reinterpret_cast<const IcmpHeader *>(ptr);
                //we hit transit router
                //or we hit target but its port/protocol is blocked
                if(
                icmp_hdr->type == ICMP_TIME_EXCEEDED ||
                (icmp_hdr->type == ICMP_UNREACH_PORT ||
                icmp_hdr->type == ICMP_UNREACH_PROTOCOL) &&
                _packet->GetDestinationAddress().IsSameAs(client)
                )
                {                                                   
                    ptr += sizeof(IcmpHeader); 
                    ptr += sizeof(Ipv4Header);
                    const TcpHeader * inner_tcp_hdr = reinterpret_cast<const TcpHeader *>(ptr);
                    if(inner_tcp_hdr->seq == _packet->GetTcpHeader().seq)
                    {
                        isResponseValid = true;
                    }                      
                }       
                break;     
            }
            case IPPROTO_ICMPV6:
            {
                const IcmpHeader * header = reinterpret_cast<const IcmpHeader *>(ptr);
                if(header->type == ICMP6_TIME_EXCEEDED ||
                (header->type == ICMP6_DST_UNREACH && 
                _packet->GetDestinationAddress().IsSameAs(client))
                )
                {
                    ptr += sizeof(IcmpHeader); 
                    ptr += Ipv6HeaderSize;
                    const TcpHeader * inner_tcp_hdr = reinterpret_cast<const TcpHeader *>(ptr);
                    if(inner_tcp_hdr->seq == _packet->GetTcpHeader().seq)
                    {
                        isResponseValid = true;
                    }        
                }   
            }
            case IPPROTO_TCP:
            {
                if(_packet->GetDestinationAddress().IsSameAs(client))
                {            
                    const TcpHeader * tcp_hdr = reinterpret_cast<const TcpHeader *>(ptr);
                    unsigned int seq = ntohl(_packet->GetTcpHeader().seq);
                    unsigned int ack_seq = ntohl(tcp_hdr->ack_seq);
                    if((ack_seq - 1) == seq )
                        isResponseValid = true;
                }     
                break;
            }
        }

        return isResponseValid;
    }
    
    
}

