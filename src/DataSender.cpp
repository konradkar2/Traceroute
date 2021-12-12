#include "Traceroute/SocketAddress.hpp"
#include "utils/Poll.hpp"
#include <Traceroute/DataSender.hpp>
#include <Traceroute/ResponseInfo.hpp>
#include <Traceroute/Socket.hpp>
#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <optional>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>

namespace traceroute {
DataSender::DataSender(std::vector<SocketExt> socketsExt, int family) : mSendingSocket{}
{
    mFamily = family;
    for (const auto &socExt : socketsExt)
    {
        if (socExt.role & Role::Receive)
        {
            mReceivingSockets.push_back(socExt.socket.fd);
        }
        if (socExt.role & Role::Send)
        {
            if (mSendingSocket != 0)
            {
                throw std::invalid_argument("There can only be one sending socket in DataSender");
            }
            mSendingSocket = socExt.socket.fd;
        }
        fdToProtocol[socExt.socket.fd] = socExt.socket.protocol;
    }
}

std::optional<ResponseInfo> DataSender::tryReceiving(char *buffer, size_t bufferSize, std::chrono::milliseconds timeout)
{
    auto fd = getAnySocketReadyToReceive(timeout);
    if (fd)
    {
        sockaddr_storage sockaddrStorage;
        socklen_t        len = sizeof(sockaddrStorage);
        ssize_t size         = ::recvfrom(fd.value(), buffer, bufferSize, 0, (struct sockaddr *)&sockaddrStorage, &len);
        if (size < 0)
        {
            throw std::runtime_error("Error occured on recvfrom: " + std::string(strerror(errno)));
        }
        return ResponseInfo(SocketAddress{std::move(sockaddrStorage)}, fdToProtocol.at(fd.value()), size);
    }
    return std::nullopt;
}

bool DataSender::trySending(const std::unique_ptr<Packet> &packet, int ttl, std::chrono::milliseconds timeout)
{
    setTtl(ttl);
    const auto &address = packet->getDestinationAddress();
    if (mFamily != address.family())
    {
        throw std::invalid_argument("Provided address's family doesn't match family of source address");
    }
    const std::string serializedPacket = packet->serialize();

    if (utils::Poll(mSendingSocket, POLLOUT, timeout))
    {
        ssize_t result = ::sendto(mSendingSocket, serializedPacket.data(), serializedPacket.size(), 0,
                                  address.sockaddrP(), address.size());
        if (result < 0)
        {
            throw std::runtime_error("Error occured while sending data " + std::string(strerror(errno)));
        }
        return static_cast<bool>(result);
    }
    return false;
}

void DataSender::setTtl(int ttl)
{
    int level  = (mFamily == AF_INET) ? (int)IPPROTO_IP : (int)IPPROTO_IPV6;
    int option = (mFamily == AF_INET) ? (int)IP_TTL : (int)IPV6_UNICAST_HOPS;
    if (setsockopt(mSendingSocket, level, option, &ttl, sizeof(ttl)) < 0)
    {
        throw std::runtime_error("Could not set ttl:" + std::string(strerror(errno)));
    }
}

std::optional<int> DataSender::getAnySocketReadyToReceive(std::chrono::milliseconds timeout)
{
    return utils::Poll(mReceivingSockets, POLLIN, timeout);
}

} // namespace traceroute