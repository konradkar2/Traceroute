#pragma once
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/SocketAddress.hpp>
namespace Traceroute
{
    class ChecksumCalculator
    {
    public:
        /*
        // all of these methods operate on network byte order !//
        */
        static unsigned short computeICMPHeaderChecksum(IcmpHeader header);
        static unsigned short computeTCPHeaderChecksum(
           TcpHeader tcpheader, const SocketAddress & source, const SocketAddress & destination );
        static unsigned short computeUdpHeaderChecksum(
           UdpHeader UdpHeader, const SocketAddress & source, const SocketAddress & destination);      
    private:
        static unsigned long computeChecksum(const unsigned short *addr, unsigned int count,
            unsigned long prevSum = 0);     
        static unsigned long computePseudoheaderChecksum(short protocol,
            short headerlen, const SocketAddress & source, const SocketAddress & destination);   
    };
}