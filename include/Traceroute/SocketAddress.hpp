#pragma once
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <arpa/inet.h>
#include <stdexcept> 
namespace Traceroute
{
    class SocketAddress 
    {
    public:
        SocketAddress(){}
        explicit SocketAddress(const sockaddr_storage & address);
        explicit SocketAddress(const std::string & address, short dport = 32424);     
        int getFamily() const;
        const sockaddr* getSockaddrP() const;       
        size_t getSize() const;      
        std::string toString() const;
        bool operator==(const SocketAddress & rhs) const;
    private:
        struct sockaddr_storage mAddress;       
    };
}