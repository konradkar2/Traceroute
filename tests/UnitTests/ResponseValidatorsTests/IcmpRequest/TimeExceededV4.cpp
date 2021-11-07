#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include <gmock/gmock.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;

namespace traceroute {
struct IcmpTimeExceededBaseV4 : public ResponseValidatorTestV4
{
    const int responseProtocol = IPPROTO_ICMP;
    const SocketAddress validResponseAddr;
    const IcmpPacket request;

    IcmpTimeExceededBaseV4()
        : ResponseValidatorTestV4(std::make_unique<responseValidators::IcmpResponseValidator>()),
          validResponseAddr(requestDestination),
          request(IcmpPacket::CreateIcmp4Packet(requestSource, requestDestination))

    {
    }
};

struct IcmpTimeExceededV4 : public IcmpTimeExceededBaseV4
{

    ResponseIcmpToIcmp<Ipv4Header> response;
    IcmpTimeExceededV4()
    {
        response.ipHeader = createStandardIpHeader();
        response.triggerPacket.ipHeader = createStandardIpHeader();
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct IcmpTimeExceededV4CustomIhl : public IcmpTimeExceededBaseV4
{
    ResponseIcmpToIcmp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    IcmpTimeExceededV4CustomIhl()
    {
        response.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.triggerPacket.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
    }
};

TEST_F(IcmpTimeExceededV4, sameId_Valid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(IcmpTimeExceededV4, differentId_Invalid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id - 1;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

TEST_F(IcmpTimeExceededV4CustomIhl, sameId_Valid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(IcmpTimeExceededV4CustomIhl, differentId_Invalid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id - 1;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest