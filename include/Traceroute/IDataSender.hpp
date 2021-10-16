#pragma once
#include <cstdint>
#include "SocketAddress.hpp"

namespace traceroute 
{
    class IDataSender
    {
    public:
        virtual int sendTo(const std::string && buffer, const SocketAddress & address)= 0;
        virtual int receiveFrom(char * buffer, size_t size, SocketAddress & address, int & protocol)= 0;   
        virtual void setTtlOnSendingSocket(int ttl)= 0;
        virtual ~IDataSender() = default;
    };
}