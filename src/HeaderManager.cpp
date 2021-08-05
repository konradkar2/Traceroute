#include <Traceroute/HeaderManager.hpp>

namespace Traceroute
{
    unsigned short HeaderManager::ComputeICMPHeaderChecksum(IcmpHeader header)
    {
        header.checksum = 0;
        const unsigned short *addr = reinterpret_cast<const unsigned short *>(&header);       
        unsigned long check = ComputeChecksum(addr,sizeof(IcmpHeader));               
        return (unsigned short)check;
    }
    unsigned long HeaderManager::ComputePseudoheaderChecksum(
        short protocol, short headerlen,const SocketAddress & source, const SocketAddress & destination)
    {
        unsigned long sum = 0;
        switch(source.GetFamily())
        {
            case AF_INET:
            {
                in_addr_t src = ((sockaddr_in *)source.GetAddress())->sin_addr.s_addr;
                sum += (src>>16)&0xFFFF;
                sum += (src)&0xFFFF;
                in_addr_t dst = ((sockaddr_in *)destination.GetAddress())->sin_addr.s_addr;
                sum += (dst>>16)&0xFFFF;
                sum += (dst)&0xFFFF;
                break;                
            }            
            case AF_INET6:
            {
                in6_addr src = ((sockaddr_in6 *)source.GetAddress())->sin6_addr;
                for(int i = 0; i< 4; i++)
                {
                    uint32_t temp = src.__in6_u.__u6_addr32[i];
                    sum += (temp>>16)&0xFFFF;
                    sum += (temp)&0xFFFF;
                }
                in6_addr dst = ((sockaddr_in6 *)source.GetAddress())->sin6_addr;
                for(int i = 0; i< 4; i++)
                {
                    uint32_t temp = dst.__in6_u.__u6_addr32[i];
                    sum += (temp>>16)&0xFFFF;
                    sum += (temp)&0xFFFF;
                }
            }              
        }
        sum += htons(protocol);
        //the length
        sum += htons(headerlen);

        return sum;
    }
    unsigned short HeaderManager::ComputeTCPHeaderChecksum(
        TcpHeader tcpheader, const SocketAddress & source, const SocketAddress & destination )
    {        
        unsigned long sum = 0;      
        short tcpLen = sizeof(TcpHeader);
        sum += ComputePseudoheaderChecksum(IPPROTO_TCP,tcpLen,source,destination); 

        tcpheader.check = 0;
        const unsigned short *ipPayload = reinterpret_cast<const unsigned short*>(&tcpheader);
        sum = ComputeChecksum(ipPayload,tcpLen,sum);        
        //set computation result
        return sum;
    }

    unsigned short HeaderManager::ComputeUdpHeaderChecksum(
        UdpHeader udpheader, const SocketAddress & source, const SocketAddress & destination )
    {
        unsigned long sum = 0;
        short udpLen = sizeof(UdpHeader);
        sum += ComputePseudoheaderChecksum(IPPROTO_UDP,udpLen,source,destination); 
        udpheader.check = 0;
        unsigned short *ipPayload = reinterpret_cast<unsigned short*>(&udpheader);
        sum = ComputeChecksum(ipPayload,udpLen,sum);                
        //set computation result
        return sum;
    }
    unsigned long HeaderManager::ComputeChecksum(const unsigned short *addr, unsigned int size, unsigned long prevSum)
    {
        unsigned long sum = prevSum;
        while (size > 1) {
            sum += * addr++;
            size -= 2;
        }
        //if any bytes left, pad the bytes and add
        if(size > 0) {
            sum += ((*addr)&htons(0xFF00));
        }
        //Fold sum to 16 bits: add carrier to result
        while (sum>>16) {
            sum = (sum & 0xffff) + (sum >> 16);
        }
        //one's complement        
        return ~sum;
    }
}