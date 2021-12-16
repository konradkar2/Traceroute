#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;

namespace traceroute {

struct TcpDestUnreachBase : public ResponseValidatorTestV4
{
    const int responseProtocol = IPPROTO_ICMP;

    const TcpPacket request = TcpPacket(requestSource, requestDestination, 80);
    TcpDestUnreachBase()
    {
        setValidator(std::make_unique<responseValidators::TcpResponseValidator>(request));
    }
};

struct TcpDestUnreachV4 : public TcpDestUnreachBase
{

    ResponseIcmpToTcp<Ipv4Header> response;
    TcpDestUnreachV4()
    {
        response.ipHeader               = createStandardIpHeader();
        response.triggerPacket.ipHeader = createStandardIpHeader();
        response.icmpHeader.type        = ICMP_DEST_UNREACH;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct TcpDestUnreachV4CustomIhl : public TcpDestUnreachBase
{
    ResponseIcmpToTcp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    TcpDestUnreachV4CustomIhl()
    {
        response.ipHeader               = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.triggerPacket.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type        = ICMP_DEST_UNREACH;
    }
};

TEST_F(TcpDestUnreachV4, valid)
{
    const SocketAddress validResponseAddr      = requestDestination;
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_TRUE(isValid);
}

TEST_F(TcpDestUnreachV4, invalidClient_invalid)
{
    const SocketAddress invalidResponseAddr{"1.1.1.1"};
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    ResponseInfo respInfo{invalidResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_FALSE(isValid);
}

TEST_F(TcpDestUnreachV4CustomIhl, valid)
{
    const SocketAddress validResponseAddr      = requestDestination;
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_TRUE(isValid);
}

TEST_F(TcpDestUnreachV4CustomIhl, invalidClient_invalid)
{
    const SocketAddress invalidResponseAddr{"1.1.1.1"};
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    ResponseInfo respInfo{invalidResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute