#include <Traceroute/DataSenderBase.hpp>
#include <string>
#include <cstdint>
#include <chrono>
#include <thread>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <poll.h>

namespace traceroute
{
    namespace
    {
        SocketInfo createIcmpSocket(const SocketAddress &addressToBind);
    }
    DataSenderBase::DataSenderBase(const SocketAddress &sourceAddr, const SocketInfo &transportProtocolSocket, std::chrono::milliseconds receiveTimeout)
    {
        mFamily = sourceAddr.family();
        mPollingTimeout = receiveTimeout;
        SocketInfo icmpSfd = createIcmpSocket(sourceAddr);
        mReceivingSockets.push_back(icmpSfd);
        if (transportProtocolSocket.sfd != 0)
            handleTransportProtocolSocket(transportProtocolSocket);
        else
            mSendingSocket = icmpSfd;

        initializePollingFds();
    }
    void DataSenderBase::handleTransportProtocolSocket(const SocketInfo &transportProtocolSocket)
    {
        mSendingSocket = transportProtocolSocket;
        if (transportProtocolSocket.isDesignatedToReceive)
            mReceivingSockets.push_back(transportProtocolSocket);
    }
    void DataSenderBase::initializePollingFds()
    {
        for (size_t i = 0; i < mReceivingSockets.size(); ++i)
        {
            mPollFds[i].fd = mReceivingSockets[i].sfd;
            mPollFds[i].events = POLLIN;
        }
    }
    int DataSenderBase::receiveFrom(char *buffer, size_t size, SocketAddress &address, int &protocol)
    {
        int n = 0;
        int sfdIndex = getIndexOfAnySocketReadyToReceive();
        if (sfdIndex >= 0)
        {
            int sfd = mReceivingSockets[sfdIndex].sfd;
            protocol = mReceivingSockets[sfdIndex].protocol;
            sockaddr_storage temp;
            socklen_t len = sizeof(temp);
            n = recvfrom(sfd, buffer, size, 0, (struct sockaddr *)&temp, &len);
            address = SocketAddress{temp};
        }
        return n;
    }

    int DataSenderBase::sendTo(const std::string &&buffer, const SocketAddress &address)
    {
        if (mFamily != address.family())
        {
            throw std::invalid_argument("Provided address's family doesn't match family of source address");
        }

        int result = sendto(mSendingSocket.sfd, buffer.c_str(), buffer.size(), 0, address.sockaddrP(), address.size());
        if (result < 0)
        {
            throw std::runtime_error("Error occured while sending data " + std::string(strerror(errno)));
        }
        return result;
    }

    void DataSenderBase::setTtlOnSendingSocket(int ttl)
    {
        int level = (mFamily == AF_INET) ? (int)IPPROTO_IP : (int)IPPROTO_IPV6;
        int option = (mFamily == AF_INET) ? (int)IP_TTL : (int)IPV6_UNICAST_HOPS;
        if (setsockopt(mSendingSocket.sfd, level, option, &ttl, sizeof(ttl)) < 0)
        {
            throw std::runtime_error("Could not set ttl:" + std::string(strerror(errno)));
        }
    }

    ssize_t DataSenderBase::getIndexOfAnySocketReadyToReceive()
    {
        int index = -1;
        int result = poll(mPollFds, 1, mPollingTimeout.count());
        if (result < 0)
        {
            throw std::runtime_error("Exception occurred while polling " + std::string(strerror(errno)));
        }       
        if (result > 0)
        {
            for (size_t i = 0; i < mReceivingSockets.size(); ++i)
            {
                if (mPollFds[i].revents == 0)
                    continue;
                else
                {
                    auto revents = mPollFds[i].revents;
                    if (revents == POLLIN)
                    {
                        index = i;
                        break;
                    }
                    else
                    {
                        throw std::runtime_error("Exception occurred while polling: got revents: " + std::to_string(revents));
                    }
                }
            }
        }
        return index;
    }

    namespace
    {
        SocketInfo createIcmpSocket(const SocketAddress &addressToBind)
        {
            int protocol = addressToBind.isV4() ? (int)IPPROTO_ICMP : (int)IPPROTO_ICMPV6;
            int sfdIcmp = socket(addressToBind.family(), SOCK_RAW | SOCK_NONBLOCK, protocol);

            if ((bind(sfdIcmp, addressToBind.sockaddrP(), addressToBind.size())) < 0)
            {
                throw std::runtime_error("Could not bind address: " + addressToBind.toString() + " bind() Error: " + std::string(strerror(errno)));
            }
            if (addressToBind.isV6())
            {
                struct icmp6_filter filter;
                ICMP6_FILTER_SETBLOCKALL(&filter);
                ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &filter);
                ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &filter);
                ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &filter);
                if (setsockopt(sfdIcmp, IPPROTO_ICMPV6, ICMP6_FILTER, &filter, sizeof(filter)) < 0)
                {
                    throw std::runtime_error("Error occured while setting icmpv6 filter: " + std::string(strerror(errno)));
                }
            }
            return {sfdIcmp, protocol, true};
        }
    }

}