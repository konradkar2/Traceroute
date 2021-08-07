#include "IcmpDataSender.hpp"
#include <netinet/in.h>

namespace Traceroute
{
    namespace Icmp
    {
        IcmpDataSender::IcmpDataSender(int family, const SocketAddress &sourceAddr, int delayMs)
            : NBlockDataSenderBase(family, sourceAddr, delayMs)
        {
        }
        int IcmpDataSender::getCurrentProtocol()
        {
            int temp = -1;
            if (mFamily == AF_INET)
                temp = IPPROTO_ICMP;
            else
                temp = IPPROTO_ICMPV6;
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

}