#pragma once
#include <cstdint>
#include "SocketAddress.hpp"

namespace Traceroute 
{
    using SA  = struct sockaddr*;
    class IDataSender
    {
    public:
        virtual int sendTo(const char * buffer, size_t size, const SocketAddress & address)= 0;
        //sets client ip addresss to 'address'
        //sets client transport protocol(or icmp/icmpv6) to 'protocol'
        virtual int receiveFrom(char * buffer, size_t size, SocketAddress & address, int & protocol)= 0;   
        virtual int getReceiveDelayMs()= 0;
        virtual void setTtl(int ttl)= 0;
        virtual ~IDataSender(){}
    };
}