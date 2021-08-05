#pragma once
#include <cstdint>
#include "SocketAddress.hpp"

namespace Traceroute 
{
    class IDataSender
    {
    public:
        virtual int SendTo(const char * buffer, size_t size, const SocketAddress & address)= 0;
        //sets client ip addresss to 'address'
        //sets client transport protocol(or icmp/icmpv6) to 'protocol'
        virtual int ReceiveFrom(char * buffer, size_t size, SocketAddress & address, int & protocol)= 0;   
        virtual int GetReceiveDelayMs()= 0;
        virtual void SetTtl(int ttl)= 0;
        virtual ~IDataSender(){}
    };
}