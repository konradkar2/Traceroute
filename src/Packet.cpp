#include <Traceroute/Packet.hpp>

namespace Traceroute{

    Packet::Packet(const SocketAddress & source,
     const SocketAddress & destination) : _saddress(source), _daddress(destination)
     {
        if(destination.GetFamily() != source.GetFamily())
        {
            throw runtime_error("source address family doesnt match destination");
        }
        
     }
    const SocketAddress & Packet::GetSourceAddress() const
    {
        return _saddress;
    }
    const SocketAddress & Packet::GetDestinationAddress() const
    {
        return _daddress;
    }
    const int & Packet::GetFamily() const
    {
        return _saddress.GetFamily();
    }
    
    void TcpPacket::Serialize(char * dataOut) const
    {
        //dataOut = Serializer<Ipv4Header>::Serialize(dataOut, _ipv4Header);
        dataOut = Serializer<TcpHeader>::Serialize(dataOut, _tcpHeader);       
    }   
    size_t TcpPacket::Serialize_size() const
    {
        return Serializer<TcpHeader>::Serialize_size(_tcpHeader);      
    }
    void UdpPacket::Serialize(char * dataOut) const
    {
        dataOut = Serializer<UdpHeader>::Serialize(dataOut, _udpHeader);       
    }
   
    size_t UdpPacket::Serialize_size() const
    {
        return Serializer<UdpHeader>::Serialize_size(_udpHeader);
               
    }

    void IcmpPacket::Serialize(char * dataOut) const
    {
        dataOut = Serializer<IcmpHeader>::Serialize(dataOut, _icmpHeader);       
    }   
    size_t IcmpPacket::Serialize_size() const
    {
        return Serializer<IcmpHeader>::Serialize_size(_icmpHeader);               
    }  
    const IcmpHeader & IcmpPacket::GetIcmpHeader() const
    {
        return _icmpHeader;
    }       
    const TcpHeader & TcpPacket::GetTcpHeader() const
    {
        return _tcpHeader;
    }  
    const UdpHeader & UdpPacket::GetUdpHeader() const
    {
        return _udpHeader;
    }
}




