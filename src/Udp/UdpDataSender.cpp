#include "UdpDataSender.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdexcept>

#define SA (struct sockaddr *)

namespace Traceroute
{
    namespace Udp
    {
        UdpDataSender::UdpDataSender(int family, const SocketAddress &sourceAddr, int delayMs)
            : NBlockDataSenderBase(family, sourceAddr, delayMs)
        {
            SfdUdp = socket(mFamily, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_UDP);
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
        int UdpDataSender::getCurrentProtocol()
        {
            int temp = -1;
            if(mFamily == AF_INET)
                temp = IPPROTO_ICMP;
            else
                temp = IPPROTO_ICMPV6;
            return temp;
        }
        int UdpDataSender::getSendingSocket()
        {
            return SfdUdp;
        }
        int UdpDataSender::getReceivingSocket()
        {
            return mSfdIcmp;
        }
    }

}