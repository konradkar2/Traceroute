#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include "Traceroute/Packet/UdpPacket.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/ResponseValidators/UdpResponseValidator.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;

namespace traceroute {
struct UdpDestUnreachBase : public ResponseValidatorTestV4
{
    const int responseProtocol = IPPROTO_ICMP;

    const UdpPacket request = UdpPacket(requestSource, requestDestination, 2223);
    UdpDestUnreachBase()
    {
        setValidator(std::make_unique<responseValidators::UdpResponseValidator>(request));
    }
};

struct UdpDestUnreachV4 : public UdpDestUnreachBase
{

    ResponseIcmpToUdp<Ipv4Header> response;
    UdpDestUnreachV4()
    {
        response.ipHeader               = createStandardIpHeader();
        response.triggerPacket.ipHeader = createStandardIpHeader();
        response.icmpHeader.type        = ICMP_DEST_UNREACH;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct UdpDestUnreachV4CustomIhl : public UdpDestUnreachBase
{
    ResponseIcmpToTcp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    UdpDestUnreachV4CustomIhl()
    {
        response.ipHeader               = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.triggerPacket.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type        = ICMP_DEST_UNREACH;
    }
};

TEST_F(UdpDestUnreachV4, valid)
{
    const SocketAddress validResponseAddr = requestDestination;

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_TRUE(isValid);
}

TEST_F(UdpDestUnreachV4, invalidClient_invalid)
{
    const SocketAddress invalidResponseAddr{"1.1.1.1"};

    ResponseInfo respInfo{invalidResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_FALSE(isValid);
}

TEST_F(UdpDestUnreachV4CustomIhl, valid)
{
    const SocketAddress validResponseAddr = requestDestination;

   ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_TRUE(isValid);
}

TEST_F(UdpDestUnreachV4CustomIhl, invalidClient_invalid)
{
    const SocketAddress invalidResponseAddr{"1.1.1.1"};

    ResponseInfo respInfo{invalidResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute