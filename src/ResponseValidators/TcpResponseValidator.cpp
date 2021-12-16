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

TcpResponseValidator::TcpResponseValidator(const packet::TcpPacket &tcpPacket)
    : mTcpToTcpValidator(std::make_unique<TcpToTcpResponseValidator>(tcpPacket)),
      mIcmpToTcpValidator(std::make_unique<IcmpToTcpResponseValidator>(tcpPacket))

{
}
bool TcpResponseValidator::isValid(const ResponseInfo &responseInfo, const char *response)
{
    auto protocol = responseInfo.protocol();
    if (protocol == IPPROTO_ICMP or protocol == IPPROTO_ICMPV6)
    {
        return mIcmpToTcpValidator->isValid(responseInfo, response);
    }
    else if (protocol == IPPROTO_TCP)
    {
        return mTcpToTcpValidator->isValid(responseInfo, response);
    }
    return false;
}

} // namespace traceroute::responseValidators
