#include <Traceroute/Packet.hpp>
#include <stdexcept>
#include "PodSerializer.hpp"
#include "ChecksumCalculator.hpp"
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
        //dataOut = PodSerializer<Ipv4Header>::Serialize(dataOut, _ipv4Header);
        dataOut = PodSerializer<TcpHeader>::serialize(dataOut, mTcpHeader);       
    }   
    size_t TcpPacket::getSerializeSize() const
    {
        return PodSerializer<TcpHeader>::getSize();      
    }
    void UdpPacket::serialize(char * dataOut) const
    {
        dataOut = PodSerializer<UdpHeader>::serialize(dataOut, mUdpHeader);       
    }
   
    size_t UdpPacket::getSerializeSize() const
    {
        return PodSerializer<UdpHeader>::getSize();
               
    }

    void IcmpPacket::serialize(char * dataOut) const
    {
        dataOut = PodSerializer<IcmpHeader>::serialize(dataOut, mIcmpHeader);       
    }   
    size_t IcmpPacket::getSerializeSize() const
    {
        return PodSerializer<IcmpHeader>::getSize();               
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




