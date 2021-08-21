#pragma once
#include "HeaderTypes.hpp"
#include "SocketAddress.hpp"

namespace Traceroute
{       
    class Packet
    {
    public:     
        Packet(const SocketAddress & source, const SocketAddress & destination);
        const SocketAddress & getSourceAddress() const;
        const SocketAddress & getDestinationAddress() const;
        int getFamily() const;
        virtual std::string serialize() const = 0;   
        virtual ~Packet(){}
    protected:
        const SocketAddress mSourceAddress;
        const SocketAddress mDestinationAddress;
    };


    class TcpPacket :  public Packet
    {
    public:
        
        TcpPacket(const SocketAddress source, const SocketAddress destination,
            TcpHeader tcpHeader) : Packet(source,destination)
        {
            mTcpHeader = tcpHeader;
        }
        TcpPacket(const SocketAddress source, const SocketAddress destination)
            : Packet(source,destination){};        

        const TcpHeader & getTcpHeader() const ;
        std::string serialize() const override;       
        ~TcpPacket() {}
    private:             
        TcpHeader mTcpHeader;
    };


    class UdpPacket : public Packet
    {
    public:
        UdpPacket(const SocketAddress source, const SocketAddress destination,
            UdpHeader udpHeader) : Packet(source,destination)
        {
            mUdpHeader = udpHeader;
        }
        UdpPacket(const SocketAddress source, const SocketAddress destination)
            : Packet(source,destination){} 
        const UdpHeader & GetUdpHeader() const ;

        std::string serialize() const override;        
        ~UdpPacket() {}
    private:     
        UdpHeader mUdpHeader;

    };


    class IcmpPacket : public Packet
    {
    public:
        IcmpPacket(const SocketAddress source, const SocketAddress destination,
            IcmpHeader icmpHeader) : Packet(source,destination)
        {
            mIcmpHeader = icmpHeader;
        }
        IcmpPacket(const SocketAddress source, const SocketAddress destination)
            : Packet(source,destination){}
        const IcmpHeader & GetIcmpHeader() const;
        std::string serialize() const override;       
        ~IcmpPacket() {}
    private:       
        IcmpHeader mIcmpHeader;
    };
}