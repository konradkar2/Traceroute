#include "utils/Poll.hpp"
#include <Traceroute/DataSender.hpp>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iterator>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <thread>
namespace traceroute
{
DataSender::DataSender(std::vector<Socket> sockets, int family, std::chrono::milliseconds pollingTimeout)
{
    mFamily = family;
    mPollingTimeout = pollingTimeout;
    for (const auto &socket : sockets)
    {
        if (socket.isReceiving)
        {
            mReceivingSockets.push_back(socket.sfd);
        }
        if (socket.isSending)
        {
            if (mSendingSocket != 0)
            {
                throw std::invalid_argument("There can be only one sending socket in DataSender");
            }
            mSendingSocket = socket.sfd;
        }
        sfdToProtocol[socket.sfd] = socket.protocol;
    }
}

int DataSender::receiveFrom(char *buffer, size_t size, SocketAddress &address, int &protocol)
{
    int n = 0;
    int sfd = getAnySocketReadyToReceive();
    if (sfd > 0)
    {
        protocol = sfdToProtocol.at(sfd);
        sockaddr_storage temp;
        socklen_t len = sizeof(temp);
        n = recvfrom(sfd, buffer, size, 0, (struct sockaddr *)&temp, &len);
        address = SocketAddress{temp};
    }
    return n;
}

int DataSender::sendTo(const std::string &&buffer, const SocketAddress &address)
{
    if (mFamily != address.family())
    {
        throw std::invalid_argument("Provided address's family doesn't match family of source address");
    }

    int result = sendto(mSendingSocket, buffer.c_str(), buffer.size(), 0, address.sockaddrP(), address.size());
    if (result < 0)
    {
        throw std::runtime_error("Error occured while sending data " + std::string(strerror(errno)));
    }
    return result;
}

void DataSender::setTtlOnSendingSocket(int ttl)
{
    int level = (mFamily == AF_INET) ? (int)IPPROTO_IP : (int)IPPROTO_IPV6;
    int option = (mFamily == AF_INET) ? (int)IP_TTL : (int)IPV6_UNICAST_HOPS;
    if (setsockopt(mSendingSocket, level, option, &ttl, sizeof(ttl)) < 0)
    {
        throw std::runtime_error("Could not set ttl:" + std::string(strerror(errno)));
    }
}

int DataSender::getAnySocketReadyToReceive()
{
    return utils::Poll(mReceivingSockets, POLLIN, mPollingTimeout);
}

} // namespace traceroute