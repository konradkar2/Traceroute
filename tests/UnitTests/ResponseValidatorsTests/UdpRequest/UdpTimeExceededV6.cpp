#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/utilsV6.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/UdpPacket.hpp>
#include <Traceroute/ResponseValidators/UdpResponseValidator.hpp>
#include <gmock/gmock.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <string>
#include <vector>

using namespace traceroute::packet;

namespace traceroute {
struct UdpTimeExceededV6 : public ResponseValidatorTestV6
{
    const int                     responseProtocol = IPPROTO_ICMPV6;
    const SocketAddress           transitRouter{"1:1:1::5"};
    const UdpPacket               request = UdpPacket(requestSource, requestDestination, 80);
    ResponseIcmpToUdp<Ipv6Header> response;
    UdpTimeExceededV6()
    {
        setValidator(std::make_unique<responseValidators::UdpResponseValidator>(request));
        response.icmpHeader.type                = ICMP6_TIME_EXCEEDED;
        response.triggerPacket.ipHeader.version = 6;
    }
};

TEST_F(UdpTimeExceededV6, valid)
{
    auto [resp, responseSize] = responseV6ToPtr(&response);
    ResponseInfo respInfo{transitRouter, responseProtocol, responseSize};
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_TRUE(isValid);
}

} // namespace traceroute