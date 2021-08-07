#include <Traceroute/SocketAddress.hpp>
#include <string>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <stdexcept> 
namespace Traceroute
{
    SocketAddress::SocketAddress(const sockaddr_storage & address)
    {
        mAddress = address;
    }
    SocketAddress::SocketAddress(const std::string & address)
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

            mAddress.ss_family = AF_INET; 
            ((sockaddr_in *)&mAddress )->sin_addr = temp;
            ((sockaddr_in *)&mAddress )->sin_port = 0;
            
            
        } else if (res->ai_family == AF_INET6) 
        {
            in6_addr temp;
            if(inet_pton(AF_INET6,address.c_str(),&temp) != 1)
            {
                throw std::runtime_error("Error occurred while converting ipv6 adress.");
            }
            mAddress.ss_family = AF_INET6; 
            ((sockaddr_in6 *)&mAddress )->sin6_addr = temp;
            ((sockaddr_in6 *)&mAddress )->sin6_port = 0;
            
        } 
        freeaddrinfo(res);
    }
    int SocketAddress::getFamily() const
    {
        return (int)mAddress.ss_family;
    }
    size_t SocketAddress::getSize() const
    {
        if(getFamily() == AF_INET)
            return sizeof(sockaddr_in);
        else 
            return sizeof(sockaddr_in6);
        

    }
    const sockaddr* SocketAddress::getSockaddrP() const
    {
        return reinterpret_cast<const sockaddr*>(&mAddress);
    }
    
    bool SocketAddress::operator==(const SocketAddress & rhs) const
    {
        switch(getFamily())
        {
            case AF_INET:
                return ((sockaddr_in *)getSockaddrP())->sin_addr.s_addr ==
                    ((sockaddr_in *)rhs.getSockaddrP())->sin_addr.s_addr;
            case AF_INET6:          
            {
                for(int i = 0; i<4 ;i++)
                {
                    uint32_t * tempA = &((sockaddr_in6 *)getSockaddrP())->sin6_addr.__in6_u.__u6_addr32[i];
                    uint32_t * tempB = &((sockaddr_in6 *)rhs.getSockaddrP())->sin6_addr.__in6_u.__u6_addr32[i];                 
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
    std::string SocketAddress::toString() const
    {
        const int bufflen = INET6_ADDRSTRLEN;
        char buffer[bufflen];
        switch(getFamily())
        {
            case AF_INET:
            {
               inet_ntop(AF_INET,&((sockaddr_in*)&mAddress)->sin_addr,buffer,bufflen);
               break;
            }
            case AF_INET6:
            {               
               inet_ntop(AF_INET6,&((sockaddr_in6*)&mAddress)->sin6_addr,buffer,bufflen);
               break;
            }
        }
        return {buffer};
    }
}