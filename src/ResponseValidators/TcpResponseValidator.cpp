#include "IcmpToTcpResponseValidator.hpp"
#include "TcpToTcpResponseValidator.hpp"
#include "common.hpp"
#include <Traceroute/Packet/TcpPacket.hpp>
#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <memory>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
namespace traceroute::responseValidators
{

TcpResponseValidator::TcpResponseValidator()
{
    mIcmpToTcpValidator = std::make_unique<IcmpToTcpResponseValidator>();
    mTcpToTcpValidator = std::make_unique<TcpToTcpResponseValidator>();
}
bool TcpResponseValidator::validate(const Packet &request, const SocketAddress &client, int protocol,
                                    const char *response, size_t responseSize)
{
    if(client.isV4())
    {
        auto ipHeaderSize = getIpHeaderSize(response);
        response += ipHeaderSize;
        responseSize -=ipHeaderSize;
    }
    switch(protocol)
    {
        case IPPROTO_ICMP:
        case IPPROTO_ICMPV6:
            return mIcmpToTcpValidator->validate(request, client, protocol, response, responseSize);
        case IPPROTO_TCP:
            return mTcpToTcpValidator->validate(request, client, protocol, response, responseSize);
    }
    return false;
}

} // namespace traceroute::responseValidators
