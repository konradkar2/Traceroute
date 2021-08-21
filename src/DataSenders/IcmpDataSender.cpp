#include <Traceroute/DataSenders/IcmpDataSender.hpp>
#include <netinet/in.h>
#include <vector>
namespace Traceroute
{
    namespace DataSenders
    {
        IcmpDataSender::IcmpDataSender(int family, const SocketAddress &sourceAddr, int delayMs)
            : DataSenderBase(family, sourceAddr, delayMs)
        {
        }
        // int IcmpDataSender::getCurrentProtocol()
        // {
        //     if (mFamily == AF_INET)
        //         temp = IPPROTO_ICMP;
        //     else
        //         temp = IPPROTO_ICMPV6;
        //     return temp;
        // }
        int IcmpDataSender::getSendingSocket()
        {
            return mSfdIcmp;
        }
        std::vector<DataSenderBase::SocketInfo> IcmpDataSender::getReceivingSockets()
        {
            SocketInfo socketInfo;
            socketInfo.sfd = mSfdIcmp;
            if (mFamily == AF_INET)
                socketInfo.protocol =  IPPROTO_ICMP;
            else
                socketInfo.protocol =  IPPROTO_ICMPV6;
            
            return std::vector{socketInfo};
        }

    }

}