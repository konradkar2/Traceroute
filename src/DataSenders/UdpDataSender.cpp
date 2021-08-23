#include <Traceroute/DataSenders/UdpDataSender.hpp>
#include <Traceroute/DataSenderBase.hpp>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdexcept>

#define SA (struct sockaddr *)

namespace Traceroute
{
    namespace DataSenders
    {
        namespace
        {
            int createUdpSocket(const SocketAddress &addressToBind);
        }
        UdpDataSender::UdpDataSender(const SocketAddress &sourceAddr, std::chrono::milliseconds receiveTimeout)
        {
            int sfdUdp = createUdpSocket(sourceAddr);
            bool isDesignatedToBeReadingSocket = false;
            SocketInfo udpSocketInfo{sfdUdp, IPPROTO_UDP, isDesignatedToBeReadingSocket};

            mDataSenderBase = std::make_unique<DataSenderBase>(sourceAddr, udpSocketInfo, receiveTimeout);
        }

        int UdpDataSender::sendTo(const std::string &&buffer, const SocketAddress &receiver)
        {
            return mDataSenderBase->sendTo(std::move(buffer), receiver);
        }
        int UdpDataSender::receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol)
        {
            return mDataSenderBase->receiveFrom(buffer, size, sender, protocol);
        }
        void UdpDataSender::setTtl(int ttl)
        {
            return mDataSenderBase->setTtl(ttl);
        }

        namespace
        {
            int createUdpSocket(const SocketAddress &addressToBind)
            {
                int sfd = socket(addressToBind.family(), SOCK_RAW, IPPROTO_UDP);
                if ((bind(sfd, SA addressToBind.sockaddrP(), addressToBind.size())) < 0)
                {
                    throw std::runtime_error("Could not bind address");
                }
                if (addressToBind.isV6())
                {
                    int offset = 6;
                    if ((setsockopt(sfd, IPPROTO_IPV6, IPV6_CHECKSUM,
                                    &offset, sizeof(offset))) < 0)
                    {
                        throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
                    }
                }
                return sfd;
            }
        } //namespae
    }

}