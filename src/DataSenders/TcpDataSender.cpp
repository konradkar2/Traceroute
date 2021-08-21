#include <Traceroute/DataSenders/TcpDataSender.hpp>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

#define SA (struct sockaddr *)

namespace Traceroute
{
    namespace DataSenders
    {
        TcpDataSender::TcpDataSender(int family, const SocketAddress &sourceAddr, int delayMs)
            : DataSenderBase(family, sourceAddr, delayMs)
        {
            mSfdTcp = socket(mFamily, SOCK_RAW , IPPROTO_TCP);

            if ((bind(mSfdTcp, sourceAddr.getSockaddrP(), sourceAddr.getSize())) < 0)
            {
                throw std::runtime_error("Could not bind address");
            }
            if (mFamily == AF_INET6)
            {
                int offset = 16;
                if ((setsockopt(mSfdTcp, IPPROTO_IPV6, IPV6_CHECKSUM,
                                &offset, sizeof(offset))) < 0)
                {
                    throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
                }
            }
        }

        // int TcpDataSender::getCurrentProtocol()
        // {
        //     int temp = -1;
        //     if (mCurrentSfd == mSfdTcp)
        //         temp = IPPROTO_TCP;
        //     else
        //     {
        //         if (mFamily == AF_INET)
        //             temp = IPPROTO_ICMP;
        //         else
        //             temp = IPPROTO_ICMPV6;
        //     }
        //     return temp;
        // }
        int TcpDataSender::getSendingSocket()
        {
            return mSfdTcp;
        }

        std::vector<DataSenderBase::SocketInfo> TcpDataSender::getReceivingSockets()
        {
            DataSenderBase::SocketInfo tcpSocketInfo;
            tcpSocketInfo.sfd = mSfdTcp;
            tcpSocketInfo.protocol = IPPROTO_TCP;
            DataSenderBase::SocketInfo icmpSocketInfo;
            icmpSocketInfo.sfd = mSfdIcmp;
            if (mFamily == AF_INET)
                icmpSocketInfo.protocol =  IPPROTO_ICMP;
            else
                icmpSocketInfo.protocol =  IPPROTO_ICMPV6;
          
            return std::vector{tcpSocketInfo,icmpSocketInfo};
        }

    }
}