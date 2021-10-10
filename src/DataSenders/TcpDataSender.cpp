#include <Traceroute/DataSenderBase.hpp>
#include <Traceroute/DataSenders/TcpDataSender.hpp>
#include <chrono>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>

namespace traceroute
{
namespace DataSenders
{
namespace
{
int createTcpSocket(const SocketAddress &addressToBind);
} // namespace
TcpDataSender::TcpDataSender(const SocketAddress &sourceAddr, std::chrono::milliseconds receiveTimeout)
{
    int sfdTcp = createTcpSocket(sourceAddr);
    bool isDesignatedToBeReadingSocket = true;
    SocketInfo tcpSocketInfo{sfdTcp, IPPROTO_TCP, isDesignatedToBeReadingSocket};

    mDataSenderBase = std::make_unique<DataSenderBase>(sourceAddr, tcpSocketInfo, receiveTimeout);
}

int TcpDataSender::sendTo(const std::string &&buffer, const SocketAddress &receiver)
{
    return mDataSenderBase->sendTo(std::move(buffer), receiver);
}

int TcpDataSender::receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol)
{
    return mDataSenderBase->receiveFrom(buffer, size, sender, protocol);
}

void TcpDataSender::setTtlOnSendingSocket(int ttl)
{
    return mDataSenderBase->setTtlOnSendingSocket(ttl);
}

namespace
{
int createTcpSocket(const SocketAddress &addressToBind)
{
    int sfd = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, IPPROTO_TCP);
    if ((bind(sfd, addressToBind.sockaddrP(), addressToBind.size())) < 0)
    {
        throw std::runtime_error("Could not bind address");
    }
    if (addressToBind.isV6())
    {
        const int TCP_CHECKSUM_OFFSET = 16;
        if ((setsockopt(sfd, IPPROTO_IPV6, IPV6_CHECKSUM, &TCP_CHECKSUM_OFFSET, sizeof(TCP_CHECKSUM_OFFSET))) < 0)
        {
            throw std::runtime_error("Could not set IPV6_CHECKSUM flag");
        }
    }
    return sfd;
}
} // namespace
} // namespace DataSenders
} // namespace traceroute