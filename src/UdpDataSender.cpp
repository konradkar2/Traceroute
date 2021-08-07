#include <Traceroute/UdpDataSender.hpp>


namespace Traceroute
{
    UdpDataSender::UdpDataSender(int family, const SocketAddress & sourceAddr,int delayMs)
    : NBlockDataSenderBase(family, sourceAddr,delayMs)
    {
        _sfd_udp = socket(mSock_family,SOCK_RAW | SOCK_NONBLOCK, IPPROTO_UDP);    
        if((bind(_sfd_udp,SA sourceAddr.getSockaddrP(),sourceAddr.getSize())) < 0)
        {
            throw std::runtime_error("Could not bind address");
        }
        if(mSock_family == AF_INET6)
        {
            int offset = 6;
            if((setsockopt(_sfd_udp, IPPROTO_IPV6, IPV6_CHECKSUM,
                &offset, sizeof(offset))) < 0)
            {
                throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
            }
        }
    }   
     int UdpDataSender::getCurrentProtocol()
    {
        int temp = -1;
        temp = mSock_family == AF_INET ? IPPROTO_ICMP : IPPROTO_ICMPV6;
        return temp;       
    }
    int UdpDataSender::getSendingSocket()
    {
       return _sfd_udp;
    }
    int UdpDataSender::getReceivingSocket()
    {
        return mSfd_icmp;
    }
    

}