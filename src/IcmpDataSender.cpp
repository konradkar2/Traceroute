#include <Traceroute/IcmpDataSender.hpp>


namespace Traceroute
{
    IcmpDataSender::IcmpDataSender(int family,const SocketAddress & sourceAddr,int delayMs)
        : NBlockDataSenderBase(family,sourceAddr,delayMs)
    {
        
    }
    int IcmpDataSender::getCurrentProtocol()
    {
        int temp = -1;
        temp = _sock_family == AF_INET ? IPPROTO_ICMP : IPPROTO_ICMPV6;
        return temp;       
    }
    
    int IcmpDataSender::getSendingSocket()
    {
        return _sfd_icmp;
    }
    int IcmpDataSender::getReceivingSocket()
    {
        return _sfd_icmp;
    }
    
    
    
   

}