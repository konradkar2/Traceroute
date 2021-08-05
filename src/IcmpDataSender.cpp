#include <Traceroute/IcmpDataSender.hpp>


namespace Traceroute
{
    IcmpDataSender::IcmpDataSender(int family,const SocketAddress & sourceAddr,int delayMs)
        : NBlockDataSenderBase(family,sourceAddr,delayMs)
    {
        
    }
    int IcmpDataSender::GetCurrentProtocol()
    {
        int temp = -1;
        temp = _sock_family == AF_INET ? IPPROTO_ICMP : IPPROTO_ICMPV6;
        return temp;       
    }
    
    int IcmpDataSender::GetSendingSocket()
    {
        return _sfd_icmp;
    }
    int IcmpDataSender::GetReceivingSocket()
    {
        return _sfd_icmp;
    }
    
    
    
   

}