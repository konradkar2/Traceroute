#include "ResponseValidators/V4/Icmp4ResponseValidator.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/V4/IpHeaderVariants.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <gmock/gmock.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::icmpRequest
{
struct TimeExceededBase : public ResponseValidatorTest
{
    const int responseProtocol = IPPROTO_ICMP;
    const SocketAddress ArbitraryDestAddr{"192.51.100.1"};
    const SocketAddress requestSource{"192.0.2.1"};
    const SocketAddress validResponseAddr = requestSource;
    const IcmpPacket request = IcmpPacket::CreateIcmp4Packet(requestSource, ArbitraryDestAddr);

    TimeExceededBase() : ResponseValidatorTest(std::make_unique<responseValidators::v4::Icmp4ResponseValidator>())
    {
    }
};

struct TimeExceededV4 : public TimeExceededBase
{

    ResponseIcmpToIcmp<Ipv4Header> response;
    TimeExceededV4()
    {
        response.ipHeader = createStandardIpHeader();
        response.triggerPacket.ipHeader = createStandardIpHeader();
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct TimeExceededV4CustomIhl : public TimeExceededBase
{
    ResponseIcmpToIcmp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    TimeExceededV4CustomIhl()
    {
        response.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.triggerPacket.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
    }
};

TEST_F(TimeExceededV4, sameId_Valid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TimeExceededV4, differentId_Invalid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id - 1;

    auto resp  = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

TEST_F(TimeExceededV4CustomIhl, sameId_Valid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TimeExceededV4CustomIhl, differentId_Invalid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id - 1;

    auto resp  = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest