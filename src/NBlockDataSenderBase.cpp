#include <Traceroute/NBlockDataSenderBase.hpp>
#include <string>
#include<netinet/in.h>
#include<sys/socket.h>
#include<chrono>
#include<thread>
#include<netinet/icmp6.h>
#include <cstdint>


namespace Traceroute
{
    NBlockDataSenderBase::NBlockDataSenderBase(int family, const SocketAddress & sourceAddr, int delayMs)
    {
        mFamily = family;
        if(mFamily != sourceAddr.getFamily())
        {
            throw std::invalid_argument("Sock family does not match SocketAddress family");
        }
        mDelayMs = delayMs;
        
        int proto = (mFamily == AF_INET) ? (int)IPPROTO_ICMP : (int)IPPROTO_ICMPV6;
        mSfdIcmp = socket(mFamily,SOCK_RAW | SOCK_NONBLOCK, proto);

        if((bind(mSfdIcmp,sourceAddr.getSockaddrP(),sourceAddr.getSize())) < 0)
        {
            throw std::runtime_error("Could not bind address: " + std::string(strerror(errno)));
        }
        if(mFamily == AF_INET6)
        {
            struct icmp6_filter myfilt;
            ICMP6_FILTER_SETBLOCKALL (&myfilt);
            //receive only echo reply, timeexceeded,dest unreachable);
            ICMP6_FILTER_SETPASS (ICMP6_ECHO_REPLY, &myfilt);
            ICMP6_FILTER_SETPASS (ICMP6_TIME_EXCEEDED, &myfilt);
            ICMP6_FILTER_SETPASS (ICMP6_DST_UNREACH, &myfilt);
            if(setsockopt(mSfdIcmp,IPPROTO_ICMPV6,ICMP6_FILTER,&myfilt,sizeof(myfilt)) < 0)
            {
                throw std::runtime_error("Error occured while setting icmpv6 filter: "+ std::string(strerror(errno)));
            }
        }
    }
    int NBlockDataSenderBase::receiveFrom(char * buffer, size_t size,SocketAddress & address, int & protocol)
    {
        sockaddr_storage temp;        
        socklen_t len = sizeof(temp);
        protocol = getCurrentProtocol();
        int n = recvfrom (getReceivingSocket(), buffer, size, 0, (SA) & temp, &len);
        if(n < 0 && errno != EAGAIN)
        {
             throw std::runtime_error("Error occured while reading data: " + std::string(strerror(errno)));
        }        
        else if(n == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(mDelayMs));
        }
        else
        {
            address = SocketAddress{temp};
        }
        return n;
    }
    void NBlockDataSenderBase::setTtl(int ttl)
    {
        switch(mFamily)
        {
            case AF_INET:
            {
               if(setsockopt(getSendingSocket(), IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl))< 0 )
               {
                   throw std::runtime_error("Could not set ttl:"  + std::string(strerror(errno)));
               }
               break;
            }
            case AF_INET6:
            {
                if(setsockopt(getSendingSocket(), IPPROTO_IPV6, IPV6_UNICAST_HOPS, &ttl, sizeof(ttl)) < 0)
               {
                   throw std::runtime_error("Could not set ttl:"  + std::string(strerror(errno)));
               }
               break;
            }
            
        }
    }
    int NBlockDataSenderBase::sendTo(const char * buffer, size_t size, const SocketAddress & address)
    {
        if(mFamily != address.getFamily())
        {
            throw std::invalid_argument("Provided address is invalid");
        }

        int result = sendto (getSendingSocket(), buffer, size, 0, address.getSockaddrP(), address.getSize());
        if(result < 0)
        {
            throw std::runtime_error("Error occured while sending data " + std::string(strerror(errno)));
        }
        return result;
        
    }
    int NBlockDataSenderBase::getReceiveDelayMs()
    {
        return mDelayMs;
    }
}