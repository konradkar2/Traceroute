#include <Traceroute/DataSenders/UdpDataSender.hpp>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdexcept>

#define SA (struct sockaddr *)

namespace Traceroute
{
    namespace DataSenders
    {
        UdpDataSender::UdpDataSender(int family, const SocketAddress &sourceAddr, int delayMs)
            : DataSenderBase(family, sourceAddr, delayMs)
        {
            SfdUdp = socket(mFamily, SOCK_RAW , IPPROTO_UDP);
            if ((bind(SfdUdp, SA sourceAddr.getSockaddrP(), sourceAddr.getSize())) < 0)
            {
                throw std::runtime_error("Could not bind address");
            }
            if (mFamily == AF_INET6)
            {
                int offset = 6;
                if ((setsockopt(SfdUdp, IPPROTO_IPV6, IPV6_CHECKSUM,
                                &offset, sizeof(offset))) < 0)
                {
                    throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
                }
            }
        }
        // int UdpDataSender::getCurrentProtocol()
        // {
        //     int temp = -1;
        //     if (mFamily == AF_INET)
        //         temp = IPPROTO_ICMP;
        //     else
        //         temp = IPPROTO_ICMPV6;
        //     return temp;
        // }
        int UdpDataSender::getSendingSocket()
        {
            return SfdUdp;
        }
        std::vector<DataSenderBase::SocketInfo>  UdpDataSender::getReceivingSockets()
        {
            SocketInfo icmpSocketInfo;
            icmpSocketInfo.sfd = mSfdIcmp;
            if (mFamily == AF_INET)
                icmpSocketInfo.protocol =  IPPROTO_ICMP;
            else
                icmpSocketInfo.protocol =  IPPROTO_ICMPV6;
            
            return std::vector{icmpSocketInfo};
        }
    }

}