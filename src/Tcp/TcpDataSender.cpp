#include "TcpDataSender.hpp"
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

#define SA (struct sockaddr *)

namespace Traceroute
{
    namespace Tcp
    {
        TcpDataSender::TcpDataSender(int family, const SocketAddress &sourceAddr, int delayMs)
            : NBlockDataSenderBase(family, sourceAddr, delayMs)
        {
            mSfdTcp = socket(mFamily, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_TCP);

            if ((bind(mSfdTcp, SA sourceAddr.getSockaddrP(), sourceAddr.getSize())) < 0)
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
            mCurrentSfd = mSfdIcmp;
        }

        int TcpDataSender::getCurrentProtocol()
        {
            int temp = -1;
            if (mCurrentSfd == mSfdTcp)
                temp = IPPROTO_TCP;
            else
            {
                if (mFamily == AF_INET)
                    temp = IPPROTO_ICMP;
                else
                    temp = IPPROTO_ICMPV6;
            }
            return temp;
        }
        int TcpDataSender::getSendingSocket()
        {
            return mSfdTcp;
        }

        int TcpDataSender::getReceivingSocket()
        {
            int temp = mCurrentSfd;

            //swap current receiving socket
            mCurrentSfd = (mCurrentSfd == mSfdIcmp) ? mSfdTcp : mSfdIcmp;

            return temp;
        }

    }
}