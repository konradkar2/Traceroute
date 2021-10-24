#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;
namespace traceroute::responseValidatorsTests::icmpRequest
{

struct EchoReplyBase : public ResponseValidatorTestV4
{
    const int responseProtocol = IPPROTO_ICMP;
    const SocketAddress validResponseAddr = requestDestination;
    const IcmpPacket request = IcmpPacket::CreateIcmp4Packet(requestSource, requestDestination);
    EchoReplyBase() : ResponseValidatorTestV4(std::make_unique<responseValidators::IcmpResponseValidator>())
    {
    }
};

struct EchoReplyV4 : public EchoReplyBase
{

    ResponseIcmpToIcmp<Ipv4Header> response;
    EchoReplyV4()
    {
        response.ipHeader = createStandardIpHeader();
        response.icmpHeader.type = ICMP_ECHOREPLY;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct EchoReplyV4CustomIhl : public EchoReplyBase
{
    ResponseIcmpToIcmp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    EchoReplyV4CustomIhl()
    {
        response.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type = ICMP_ECHOREPLY;
    }
};

TEST_F(EchoReplyV4, SameId_Valid)
{
    response.icmpHeader.id = request.GetIcmpHeader().id;

    auto resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(EchoReplyV4, differentId_Invalid)
{
    response.icmpHeader.id = request.GetIcmpHeader().id - 1;

    auto resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, responseSize);

    EXPECT_FALSE(isValid);
}

TEST_F(EchoReplyV4CustomIhl, sameId_Valid)
{
    response.icmpHeader.id = request.GetIcmpHeader().id;

    auto resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, responseSize);

    EXPECT_TRUE(isValid);
}

TEST_F(EchoReplyV4CustomIhl, differentId_Invalid)
{
    response.icmpHeader.id = request.GetIcmpHeader().id - 1;

    auto resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest