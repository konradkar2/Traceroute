#include <Traceroute/IcmpDataSender.hpp>
#include <netinet/in.h>

namespace Traceroute
{
    IcmpDataSender::IcmpDataSender(int family,const SocketAddress & sourceAddr,int delayMs)
        : NBlockDataSenderBase(family,sourceAddr,delayMs)
    {
        
    }
    int IcmpDataSender::getCurrentProtocol()
    {
        int temp = -1;
        temp = (mFamily == AF_INET ? IPPROTO_ICMP : IPPROTO_ICMPV6);
        return temp;       
    }
    
    int IcmpDataSender::getSendingSocket()
    {
        return mSfdIcmp;
    }
    int IcmpDataSender::getReceivingSocket()
    {
        return mSfdIcmp;
    }
    
    
    
   

}