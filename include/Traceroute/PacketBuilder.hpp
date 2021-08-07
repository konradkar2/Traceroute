#pragma once
#include "Packet.hpp"
#include "ChecksumCalculator.hpp"

using namespace std;
namespace Traceroute
{
    class PacketBuilder
    {
    public:
        static IcmpPacket  CreateIcmpPacket(const SocketAddress & source,
             const SocketAddress & destination);       
        static TcpPacket  CreateTcpPacket(const SocketAddress & source,
             const SocketAddress  & destination,  int dport = 80);
        static UdpPacket  CreateUdpPacket(const SocketAddress & source,
            const SocketAddress & destination, int dport = 35555);
    
    private:
        static IcmpPacket  CreateIcmp4Packet(const SocketAddress & source,
             const SocketAddress & destination);       
        static IcmpPacket  CreateIcmp6Packet(const SocketAddress & source,
            const SocketAddress  & destination);        
       
    };
}