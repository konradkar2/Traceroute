#include <Traceroute/SocketAddress.hpp>

namespace Traceroute
{
    SocketAddress::SocketAddress(const std::string & address, short dport)
    {
        struct addrinfo hint, *res = NULL;
        int ret;

        memset(&hint, '\0', sizeof hint);

        hint.ai_family = PF_UNSPEC;
        hint.ai_flags = AI_NUMERICHOST;

        ret = getaddrinfo(address.c_str(), NULL, &hint, &res);
        if (ret || (res->ai_family != AF_INET && res->ai_family != AF_INET6)) 
        {
            throw std::invalid_argument("Invalid address: " + address );
        }
        if(res->ai_family == AF_INET) 
        {
            in_addr temp;
            if(inet_pton(AF_INET,address.c_str(),&temp) != 1)
            {
                throw std::runtime_error("Error occurred while converting ipv4 adress.");
            }

            _address.ss_family = AF_INET; 
            ((sockaddr_in *)&_address )->sin_addr = temp;
            ((sockaddr_in *)&_address )->sin_port = htons(dport);
            
            
        } else if (res->ai_family == AF_INET6) 
        {
            in6_addr temp;
            if(inet_pton(AF_INET6,address.c_str(),&temp) != 1)
            {
                throw std::runtime_error("Error occurred while converting ipv6 adress.");
            }
            _address.ss_family = AF_INET6; 
            ((sockaddr_in6 *)&_address )->sin6_addr = temp;
            ((sockaddr_in6 *)&_address )->sin6_port = htons(dport);
            
        } 
        freeaddrinfo(res);
    }
    int SocketAddress::GetFamily() const
    {
        return (int)_address.ss_family;
    }
    size_t SocketAddress::GetSize() const
    {
        if(GetFamily() == AF_INET)
            return sizeof(sockaddr_in);
        else 
            return sizeof(sockaddr_in6);
        

    }
    const sockaddr_storage * SocketAddress::GetAddress() const
    {
        return &_address;
    }

    void SocketAddress::SetAddress(const sockaddr_storage * addr)
    {
        memcpy(&_address, addr, sizeof(sockaddr_storage));
    }
    bool SocketAddress::IsSameAs(const SocketAddress & addr) const
    {
        switch(GetFamily())
        {
            case AF_INET:
                return ((sockaddr_in *)addr.GetAddress())->sin_addr.s_addr ==
                    ((sockaddr_in *)GetAddress())->sin_addr.s_addr;
            case AF_INET6:          
            {
                for(int i = 0; i<4 ;i++)
                {
                    uint32_t * tempA = &((sockaddr_in6 *)addr.GetAddress())->sin6_addr.__in6_u.__u6_addr32[i];
                    uint32_t * tempB = &((sockaddr_in6 *)GetAddress())->sin6_addr.__in6_u.__u6_addr32[i];                 
                    if(*tempA != *tempB)
                    {
                        return false;
                    }
                }
                return true;
                
            }
            
        }
        return false;
    }
    std::string SocketAddress::ToString() const
    {
        const int bufflen = INET6_ADDRSTRLEN;
        char buffer[bufflen];
        switch(GetFamily())
        {
            case AF_INET:
            {
               inet_ntop(AF_INET,&((sockaddr_in*)&_address)->sin_addr,buffer,bufflen);
               break;
            }
            case AF_INET6:
            {               
               inet_ntop(AF_INET6,&((sockaddr_in6*)&_address)->sin6_addr,buffer,bufflen);
               break;
            }
        }
        return {buffer};
    }
}