#include <Traceroute/PacketBuilder.hpp>



namespace Traceroute{
    IcmpPacket PacketBuilder::CreateIcmpPacket(const SocketAddress & source, const SocketAddress & destination)
    {
        int family = source.getFamily();
        switch(family)
        {
            case AF_INET:
                return CreateIcmp4Packet(source,destination);
            case AF_INET6:
                return CreateIcmp6Packet(source,destination);
            default:
            {
                throw invalid_argument("Invalid family address");
            }
        }
       
    }
    IcmpPacket PacketBuilder::CreateIcmp4Packet(const SocketAddress & source, const SocketAddress & destination)
    {
        IcmpHeader icmp_hdr;
        icmp_hdr.type = ICMP_ECHO;
        icmp_hdr.id = htons(rand() % 65536);  // random id
        icmp_hdr.sequence = 0;
        icmp_hdr.code = 0;        
        icmp_hdr.checksum = HeaderManager::computeICMPHeaderChecksum(icmp_hdr);
        IcmpPacket packet(source,destination, icmp_hdr);
        return packet;
    }
    IcmpPacket PacketBuilder::CreateIcmp6Packet(const SocketAddress & source, const SocketAddress & destination)
    {
        IcmpHeader icmp_hdr;
        icmp_hdr.type = ICMP6_ECHO_REQUEST;
        icmp_hdr.id = htons(rand() % 65536);  // random id
        icmp_hdr.sequence = 0;
        icmp_hdr.code = 0;        
        icmp_hdr.checksum = 0;// leave it to kernel
        IcmpPacket packet(source,destination, icmp_hdr);
        
        return packet;
    }
    TcpPacket PacketBuilder::CreateTcpPacket(const SocketAddress & source,
         const SocketAddress & destination, int dport)
    {
        TcpHeader tcp_hdr;
        tcp_hdr.source= htons(50000 + rand() % 15001);
        int temp = ntohs(tcp_hdr.source);
        tcp_hdr.dest = htons(dport);
        tcp_hdr.seq = htonl(rand() % 65001); // generate random sequence number
        tcp_hdr.ack_seq = 0; // ack sequence should be 0
        tcp_hdr.window = htons(5760); // set window size        
        tcp_hdr.th_x2 = 0;
        tcp_hdr.th_off = 5;
        tcp_hdr.th_flags = 0x02;   
        tcp_hdr.check = 0;  
        if(source.getFamily() == AF_INET)
        {
            tcp_hdr.check = HeaderManager::computeTCPHeaderChecksum(tcp_hdr,source,destination);
            //leave it to kernel if AF_INET6
        }
            
        
        TcpPacket packet(source,destination,tcp_hdr);
        
        return packet;
    }
    UdpPacket PacketBuilder::CreateUdpPacket(const SocketAddress & source,
         const SocketAddress & destination ,int dport)
    {     
        UdpHeader udp_hdr;
        udp_hdr.source= htons(50000 + rand() % 15001);
        udp_hdr.dest = htons(dport);
        udp_hdr.len = htons(sizeof(udp_hdr));
        udp_hdr.check = 0;
        if(source.getFamily() == AF_INET)
        {
            udp_hdr.check = HeaderManager::computeUdpHeaderChecksum(udp_hdr,source,destination);
            //leave it to kernel if AF_INET6
        }            
        
        UdpPacket packet(source,destination,udp_hdr);
        
        return packet;
    }
   
    
}
