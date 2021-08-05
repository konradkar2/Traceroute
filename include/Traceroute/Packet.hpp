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
        const SocketAddress & GetSourceAddress() const;
        const SocketAddress & GetDestinationAddress() const;
        const int & GetFamily() const;
        virtual std::size_t Serialize_size() const = 0;
        virtual void Serialize(char* dataOut) const = 0;   
        virtual ~Packet(){}
    protected:
        const SocketAddress _saddress;
        const SocketAddress _daddress;
    };


    class TcpPacket :  public Packet
    {
    public:
        
        TcpPacket(const SocketAddress source, const SocketAddress destination,
            TcpHeader tcpHeader) : Packet(source,destination)
        {
            _tcpHeader = tcpHeader;
        }
        TcpPacket(const SocketAddress source, const SocketAddress destination)
            : Packet(source,destination){};        

        const TcpHeader & GetTcpHeader() const ;
        std::size_t Serialize_size() const override;
        void Serialize(char* dataOut) const override;       
        ~TcpPacket() {}
    private:             
        TcpHeader _tcpHeader;
    };


    class UdpPacket : public Packet
    {
    public:
        UdpPacket(const SocketAddress source, const SocketAddress destination,
            UdpHeader udpHeader) : Packet(source,destination)
        {
            _udpHeader = udpHeader;
        }
        UdpPacket(const SocketAddress source, const SocketAddress destination)
            : Packet(source,destination){} 
        const UdpHeader & GetUdpHeader() const ;

        std::size_t Serialize_size() const override;
        void Serialize(char* dataOut) const override;        
        ~UdpPacket() {}
    private:     
        UdpHeader _udpHeader;

    };


    class IcmpPacket : public Packet
    {
    public:
        IcmpPacket(const SocketAddress source, const SocketAddress destination,
            IcmpHeader icmpHeader) : Packet(source,destination)
        {
            _icmpHeader = icmpHeader;
        }
        IcmpPacket(const SocketAddress source, const SocketAddress destination)
            : Packet(source,destination){}
        const IcmpHeader & GetIcmpHeader() const;
        std::size_t Serialize_size() const override;
        void Serialize(char* dataOut) const override;       
        ~IcmpPacket() {}
    private:       
        IcmpHeader _icmpHeader;
    };
}