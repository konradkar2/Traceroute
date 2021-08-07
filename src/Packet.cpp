#include <Traceroute/Packet.hpp>
#include <stdexcept>
namespace Traceroute{

    Packet::Packet(const SocketAddress & source,
     const SocketAddress & destination) : mSourceAddress(source), mDestinationAddress(destination)
     {
        if(destination.getFamily() != source.getFamily())
        {
            throw runtime_error("source address family doesnt match destination");
        }
        
     }
    const SocketAddress & Packet::getSourceAddress() const
    {
        return mSourceAddress;
    }
    const SocketAddress & Packet::getDestinationAddress() const
    {
        return mDestinationAddress;
    }
    int Packet::getFamily() const
    {
        return mSourceAddress.getFamily();
    }
    
    void TcpPacket::serialize(char * dataOut) const
    {
        //dataOut = Serializer<Ipv4Header>::Serialize(dataOut, _ipv4Header);
        dataOut = Serializer<TcpHeader>::serialize(dataOut, mTcpHeader);       
    }   
    size_t TcpPacket::getSerializeSize() const
    {
        return Serializer<TcpHeader>::getSize(mTcpHeader);      
    }
    void UdpPacket::serialize(char * dataOut) const
    {
        dataOut = Serializer<UdpHeader>::serialize(dataOut, mUdpHeader);       
    }
   
    size_t UdpPacket::getSerializeSize() const
    {
        return Serializer<UdpHeader>::getSize(mUdpHeader);
               
    }

    void IcmpPacket::serialize(char * dataOut) const
    {
        dataOut = Serializer<IcmpHeader>::serialize(dataOut, mIcmpHeader);       
    }   
    size_t IcmpPacket::getSerializeSize() const
    {
        return Serializer<IcmpHeader>::getSize(mIcmpHeader);               
    }  
    const IcmpHeader & IcmpPacket::GetIcmpHeader() const
    {
        return mIcmpHeader;
    }       
    const TcpHeader & TcpPacket::getTcpHeader() const
    {
        return mTcpHeader;
    }  
    const UdpHeader & UdpPacket::GetUdpHeader() const
    {
        return mUdpHeader;
    }
}




