#pragma once
#include "Serializable.hpp"
#include "HeaderTypes.hpp"
#include "Packet.hpp"
#include "SocketAddress.hpp"
namespace Traceroute
{
    class HeaderManager
    {
    public:
        /*
        // all of these methods operate on network byte order !//
        */
        static unsigned short ComputeICMPHeaderChecksum(IcmpHeader header);
        static unsigned short ComputeTCPHeaderChecksum(
           TcpHeader tcpheader, const SocketAddress & source, const SocketAddress & destination );
        static unsigned short ComputeUdpHeaderChecksum(
           UdpHeader UdpHeader, const SocketAddress & source, const SocketAddress & destination);      
    private:
        static unsigned long ComputeChecksum(const unsigned short *addr, unsigned int count,
            unsigned long prevSum = 0);     
        static unsigned long ComputePseudoheaderChecksum(short protocol,
            short headerlen, const SocketAddress & source, const SocketAddress & destination);   
    };
}