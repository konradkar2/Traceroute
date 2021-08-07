#pragma once
#include "Packet.hpp"
#include "SocketAddress.hpp"
#include <cstdint>
namespace Traceroute 
{
    class IValidateResponse
    {
    public:
        virtual bool isResponseValid(const Packet & request, const SocketAddress &client,
                     int protocol, const char * response, size_t responseSize) = 0;
        virtual ~IValidateResponse(){}
    };
}