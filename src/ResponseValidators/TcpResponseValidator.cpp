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
namespace traceroute::responseValidators {

TcpResponseValidator::TcpResponseValidator(const packet::TcpPacket &tcpPacket) : mTcpPacket(tcpPacket)
{
}
bool TcpResponseValidator::validate(const ResponseInfo &responseInfo, const char *response)
{
    auto protocol = responseInfo.protocol();
    if (protocol == IPPROTO_ICMP or protocol == IPPROTO_ICMPV6)
    {
        IcmpToTcpResponseValidator validator{mTcpPacket};
        return validator.validate(responseInfo, response);
    }
    else if (protocol == IPPROTO_TCP)
    {
        TcpToTcpResponseValidator validator{mTcpPacket};
        return validator.validate(responseInfo, response);
    }
    return false;
}

} // namespace traceroute::responseValidators
