#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <netinet/icmp6.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::tcpRequest
{
struct DestUnreachBase : public ResponseValidatorTestV4
{
    const int responseProtocol = IPPROTO_ICMP;
    
    const TcpPacket request = TcpPacket(requestSource,requestDestination,80);
    DestUnreachBase() : ResponseValidatorTestV4(std::make_unique<responseValidators::TcpResponseValidator>())
    {
    }
};

struct DestUnreachV4 : public DestUnreachBase
{

    ResponseIcmpToTcp<Ipv4Header> response;
    DestUnreachV4()
    {
        response.ipHeader = createStandardIpHeader();
        response.triggerPacket.ipHeader = createStandardIpHeader();
        response.icmpHeader.type = ICMP_DEST_UNREACH;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct DestUnreachV4CustomIhl : public DestUnreachBase
{
    ResponseIcmpToTcp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    DestUnreachV4CustomIhl()
    {
        response.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.triggerPacket.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type = ICMP_DEST_UNREACH;
    }
};

TEST_F(DestUnreachV4, valid)
{
    const SocketAddress validResponseAddr = requestDestination;
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}

TEST_F(DestUnreachV4, invalidClient_invalid)
{
    const SocketAddress invalidResponseAddr{"1.1.1.1"};
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, invalidResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}


TEST_F(DestUnreachV4CustomIhl, valid)
{
    const SocketAddress validResponseAddr = requestDestination;
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}

TEST_F(DestUnreachV4CustomIhl, invalidClient_invalid)
{
    const SocketAddress invalidResponseAddr{"1.1.1.1"};
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, invalidResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest