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
        SocketAddress(){};
        SocketAddress(const std::string & address, short dport = 32424);     
        void SetAddress(const sockaddr_storage * addr);
        int GetFamily() const;
        const sockaddr_storage * GetAddress() const;       
        size_t GetSize() const;       
        bool IsSameAs(const SocketAddress & addr) const;
        std::string ToString() const;
    private:
        struct sockaddr_storage _address;       
    };
}