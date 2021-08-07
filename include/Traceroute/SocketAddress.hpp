#pragma once
#include <string>
#include <sys/socket.h>



namespace Traceroute
{
    class SocketAddress 
    {
    public:
        SocketAddress(){}
        explicit SocketAddress(const sockaddr_storage & address);
        explicit SocketAddress(const std::string & address);     
        int getFamily() const;
        const sockaddr* getSockaddrP() const;       
        size_t getSize() const;      
        std::string toString() const;
        bool operator==(const SocketAddress & rhs) const;
    private:
        struct sockaddr_storage mAddress;       
    };
}