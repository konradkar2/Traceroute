#pragma once
#include <cstdint>
#include "SocketAddress.hpp"

namespace Traceroute 
{
    class IDataSender
    {
    public:
        virtual int sendTo(const std::string && buffer, const SocketAddress & address)= 0;
        virtual int receiveFrom(char * buffer, size_t size, SocketAddress & address, int & protocol)= 0;   
        virtual int getReceiveDelayMs()= 0;
        virtual void setTtl(int ttl)= 0;
        virtual ~IDataSender(){}
    };
}