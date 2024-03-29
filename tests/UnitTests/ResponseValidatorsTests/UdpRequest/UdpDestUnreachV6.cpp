#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/utilsV6.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/UdpPacket.hpp>
#include <Traceroute/ResponseValidators/UdpResponseValidator.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <netinet/icmp6.h>

using namespace traceroute::packet;

namespace traceroute {
struct UdpDestUnreachV6 : public ResponseValidatorTestV6
{
    const int           responseProtocol  = IPPROTO_ICMPV6;
    const SocketAddress validResponseAddr = requestDestination;

    const UdpPacket               request = UdpPacket(requestSource, requestDestination, 1234);
    ResponseIcmpToTcp<Ipv6Header> response;
    UdpDestUnreachV6()
    {
        setValidator(std::make_unique<responseValidators::UdpResponseValidator>(request));
        response.icmpHeader.type                = ICMP6_DST_UNREACH;
        response.triggerPacket.ipHeader.version = 6;
    }
};

TEST_F(UdpDestUnreachV6, valid)
{
    auto [resp, responseSize] = responseV6ToPtr(&response);
    ResponseInfo respInfo{validResponseAddr, responseProtocol, responseSize};
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_TRUE(isValid);
}

TEST_F(UdpDestUnreachV6, invalidClient)
{
    const SocketAddress invalidResponseAddr{"a:b:c::1"};

    auto [resp, responseSize] = responseV6ToPtr(&response);
    ResponseInfo respInfo{invalidResponseAddr, responseProtocol, responseSize};
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute