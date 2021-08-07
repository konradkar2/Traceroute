#include "Packet.hpp"
#include "SocketAddress"
#include <cstdint>
namespace Traceroute 
{
    class IResponseValidate
    {
        virtual bool ValidateResponse(const Packet & request, const SocketAddress &client,
                     int protocol, const char * response, size_t responseSize) = 0;
    }
}