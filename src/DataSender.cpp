#include "Traceroute/SocketAddress.hpp"
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
#include <sys/poll.h>
#include <sys/socket.h>
#include <thread>
namespace traceroute
{
DataSender::DataSender(std::vector<Socket> sockets, int family)
{
    mFamily = family;
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

ResponseInfo DataSender::receiveFrom(char *buffer, size_t bufferSize, std::chrono::milliseconds timeout)
{
    ResponseInfo info;
    info.size = 0;
    int sfd = getAnySocketReadyToReceive(timeout);
    if (sfd > 0)
    {
        info.protocol = sfdToProtocol.at(sfd);
        sockaddr_storage temp;
        socklen_t len = sizeof(temp);
        info.size = recvfrom(sfd, buffer, bufferSize, 0, (struct sockaddr *)&temp, &len);
        info.client = SocketAddress{temp};
    }
    return info;
}

int DataSender::sendPacket(const Packet &packet)
{
    const auto &address = packet.getDestinationAddress();
    if (mFamily != address.family())
    {
        throw std::invalid_argument("Provided address's family doesn't match family of source address");
    }
    auto serializedPacket = packet.serialize();
    int result = sendto(mSendingSocket, serializedPacket.data(), serializedPacket.size(), 0, address.sockaddrP(),
                        address.size());
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

int DataSender::getAnySocketReadyToReceive(std::chrono::milliseconds timeout)
{
    std::vector<pollfd> pollfds;
    std::transform(mReceivingSockets.cbegin(), mReceivingSockets.cend(), back_inserter(pollfds), [](int sfd) {
        pollfd pollfd;
        pollfd.events = POLLIN;
        pollfd.fd = sfd;
        return pollfd;
    });
    return utils::Poll(pollfds, timeout);
}

} // namespace traceroute