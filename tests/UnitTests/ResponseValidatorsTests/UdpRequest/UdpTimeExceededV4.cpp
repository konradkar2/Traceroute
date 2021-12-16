#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/UdpPacket.hpp>
#include <Traceroute/ResponseValidators/UdpResponseValidator.hpp>
#include <gmock/gmock.h>
#include <netinet/in.h>
#include <string>
#include <vector>

using namespace traceroute::packet;

namespace traceroute {
struct UdpTimeExceededBaseV4 : public ResponseValidatorTestV4
{
    const int           responseProtocol = IPPROTO_ICMP;
    const SocketAddress transitRouter{"23.42.42.42"};

    const UdpPacket request = UdpPacket(requestSource, requestDestination, 80);
    UdpTimeExceededBaseV4()
    {
        setValidator(std::make_unique<responseValidators::UdpResponseValidator>(request));
    }
};

struct UdpTimeExceededV4 : public UdpTimeExceededBaseV4
{

    ResponseIcmpToUdp<Ipv4Header> response;
    UdpTimeExceededV4()
    {
        response.ipHeader               = createStandardIpHeader();
        response.triggerPacket.ipHeader = createStandardIpHeader();
        response.icmpHeader.type        = ICMP_TIME_EXCEEDED;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct UdpTimeExceededV4CustomIhl : public UdpTimeExceededBaseV4
{
    ResponseIcmpToUdp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    UdpTimeExceededV4CustomIhl()
    {
        response.ipHeader               = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.triggerPacket.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type        = ICMP_TIME_EXCEEDED;
    }
};

TEST_F(UdpTimeExceededV4, valid)
{
    ResponseInfo respInfo{transitRouter, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_TRUE(isValid);
}

TEST_F(UdpTimeExceededV4CustomIhl, valid)
{
    ResponseInfo respInfo{transitRouter, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_TRUE(isValid);
}

} // namespace traceroute