#pragma once
#include "HeaderTypes.hpp"
#include "Serializable.hpp"
#include "HeaderManager.hpp"
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
        virtual std::size_t getSerializeSize() const = 0;
        virtual void serialize(char* dataOut) const = 0;   
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
        std::size_t getSerializeSize() const override;
        void serialize(char* dataOut) const override;       
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

        std::size_t getSerializeSize() const override;
        void serialize(char* dataOut) const override;        
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
        std::size_t getSerializeSize() const override;
        void serialize(char* dataOut) const override;       
        ~IcmpPacket() {}
    private:       
        IcmpHeader mIcmpHeader;
    };
}