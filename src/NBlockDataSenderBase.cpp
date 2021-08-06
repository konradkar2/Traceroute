#include <Traceroute/NBlockDataSenderBase.hpp>

namespace Traceroute
{
    NBlockDataSenderBase::NBlockDataSenderBase(int family, const SocketAddress & sourceAddr, int delayMs)
    {
        _sock_family = family;
        if(_sock_family != sourceAddr.getFamily())
        {
            throw std::invalid_argument("Sock family does not match SocketAddress family");
        }
        _delayMs = delayMs;
        
        int proto = (_sock_family == AF_INET) ? IPPROTO_ICMP : IPPROTO_ICMPV6;
        _sfd_icmp = socket(_sock_family,SOCK_RAW | SOCK_NONBLOCK, proto);

        if((bind(_sfd_icmp,sourceAddr.getSockaddrP(),sourceAddr.getSize())) < 0)
        {
            throw std::runtime_error("Could not bind address: " + std::string(strerror(errno)));
        }
        if(_sock_family == AF_INET6)
        {
            struct icmp6_filter myfilt;
            ICMP6_FILTER_SETBLOCKALL (&myfilt);
            //receive only echo reply, timeexceeded,dest unreachable);
            ICMP6_FILTER_SETPASS (ICMP6_ECHO_REPLY, &myfilt);
            ICMP6_FILTER_SETPASS (ICMP6_TIME_EXCEEDED, &myfilt);
            ICMP6_FILTER_SETPASS (ICMP6_DST_UNREACH, &myfilt);
            if(setsockopt(_sfd_icmp,IPPROTO_ICMPV6,ICMP6_FILTER,&myfilt,sizeof(myfilt)) < 0)
            {
                throw std::runtime_error("Error occured while setting icmpv6 filter: "+ std::string(strerror(errno)));
            }
        }
    }
    int NBlockDataSenderBase::ReceiveFrom(char * buffer, size_t size,SocketAddress & address, int & protocol)
    {
        sockaddr_storage temp;        
        socklen_t len = sizeof(temp);
        protocol = getCurrentProtocol();
        int n = recvfrom (getReceivingSocket(), buffer, size, 0, SA & temp, &len);
        if(n < 0 && errno != EAGAIN)
        {
             throw std::runtime_error("Error occured while reading data: " + std::string(strerror(errno)));
        }        
        else if(n == 0)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(_delayMs));
        }
        else
        {
            address = SocketAddress{temp};
        }
        return n;
    }
    void NBlockDataSenderBase::SetTtl(int ttl)
    {
        switch(_sock_family)
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
    int NBlockDataSenderBase::SendTo(const char * buffer, size_t size, const SocketAddress & address)
    {
        if(_sock_family != address.getFamily())
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
    int NBlockDataSenderBase::GetReceiveDelayMs()
    {
        return _delayMs;
    }
}