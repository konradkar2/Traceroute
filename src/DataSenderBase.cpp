#include <Traceroute/DataSenderBase.hpp>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <chrono>
#include <thread>
#include <netinet/icmp6.h>
#include <cstdint>
#include <poll.h>
#include <array>

#define SA (struct sockaddr *)
namespace Traceroute
{
    DataSenderBase::DataSenderBase(int family, const SocketAddress &sourceAddr, int delayMs)
    {
        mFamily = family;
        if (mFamily != sourceAddr.getFamily())
        {
            throw std::invalid_argument("Sock family does not match SocketAddress family");
        }
        mDelayMs = delayMs;

        int proto = (mFamily == AF_INET) ? (int)IPPROTO_ICMP : (int)IPPROTO_ICMPV6;
        mSfdIcmp = socket(mFamily, SOCK_RAW | SOCK_NONBLOCK, proto);

        if ((bind(mSfdIcmp, sourceAddr.getSockaddrP(), sourceAddr.getSize())) < 0)
        {
            throw std::runtime_error("Could not bind address: " + sourceAddr.toString() + " bind() Error: " + std::string(strerror(errno)));
        }
        if (mFamily == AF_INET6)
        {
            struct icmp6_filter filter;
            ICMP6_FILTER_SETBLOCKALL(&filter);
            ICMP6_FILTER_SETPASS(ICMP6_ECHO_REPLY, &filter);
            ICMP6_FILTER_SETPASS(ICMP6_TIME_EXCEEDED, &filter);
            ICMP6_FILTER_SETPASS(ICMP6_DST_UNREACH, &filter);
            if (setsockopt(mSfdIcmp, IPPROTO_ICMPV6, ICMP6_FILTER, &filter, sizeof(filter)) < 0)
            {
                throw std::runtime_error("Error occured while setting icmpv6 filter: setsockopt(..): " + std::string(strerror(errno)));
            }
        }
    }
   
    int DataSenderBase::receiveFrom(char *buffer, size_t size, SocketAddress &address, int &protocol)
    {
        int n = 0;
        std::vector<SocketInfo> socketInfos = getReceivingSockets();
        std::vector<struct pollfd> pollfds;
        for(auto socketInfo : socketInfos)
        {
            struct pollfd fd;
            fd.fd = socketInfo.sfd;
            fd.events = POLLIN;
            pollfds.push_back(fd);
        }
        ret = poll(pollfds, 1, 100); 
        switch (ret)
        {
        case -1:
            throw std::runtime_error("Error occured on polling: " + std::string(strerror(errno)));
            break;
        case 0:
            break;
        default:
            sockaddr_storage temp;
            socklen_t len = sizeof(temp);
            n = recvfrom(fd[0].fd, buffer, size, 0, SA & temp, &len);
            address = SocketAddress{temp};
        }
        return n;
    }
    void DataSenderBase::setTtl(int ttl)
    {
        switch (mFamily)
        {
        case AF_INET:
        {
            if (setsockopt(getSendingSocket(), IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0)
            {
                throw std::runtime_error("Could not set ttl:" + std::string(strerror(errno)));
            }
            break;
        }
        case AF_INET6:
        {
            if (setsockopt(getSendingSocket(), IPPROTO_IPV6, IPV6_UNICAST_HOPS, &ttl, sizeof(ttl)) < 0)
            {
                throw std::runtime_error("Could not set ttl:" + std::string(strerror(errno)));
            }
            break;
        }
        }
    }
    int DataSenderBase::sendTo(const std::string &&buffer, const SocketAddress &address)
    {
        if (mFamily != address.getFamily())
        {
            throw std::invalid_argument("Provided address is invalid");
        }

        int result = sendto(getSendingSocket(), buffer.c_str(), buffer.size(), 0, address.getSockaddrP(), address.getSize());
        if (result < 0)
        {
            throw std::runtime_error("Error occured while sending data " + std::string(strerror(errno)));
        }
        return result;
    }
    int DataSenderBase::getReceiveDelayMs()
    {
        return mDelayMs;
    }
}