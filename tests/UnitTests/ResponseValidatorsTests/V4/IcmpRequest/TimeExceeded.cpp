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

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::icmpRequest
{

struct TimeExceededV4 : public ::testing::Test
{
    ResponseIcmpToIcmp<Ipv4Header> response;
    const int responseProtocol = IPPROTO_ICMP;
    const SocketAddress ArbitraryDestAddr{"192.51.100.1"};
    std::unique_ptr<traceroute::IValidateResponse> validator =
        std::make_unique<responseValidators::v4::Icmp4ResponseValidator>();
    const SocketAddress requestSource{"192.0.2.1"};
    const SocketAddress validResponseAddr = requestSource;
    const IcmpPacket request = IcmpPacket::CreateIcmp4Packet(requestSource, ArbitraryDestAddr);

    void SetUp() override
    {
        response.ipHeader.ihl = 5;
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
        response.triggerPacket.ipHeader.ihl = 5;
        response.triggerPacket.transportHeader = request.GetIcmpHeader();
    }
};

TEST_F(TimeExceededV4, ProperResponseAddr_InnerIcmpSameId_Valid)
{
    ResponseIcmpToIcmp<Ipv4Header> response;
    response.ipHeader = createStandardIpHeader();
    const char *resp = reinterpret_cast<const char *>(&response);

    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TimeExceededV4, ProperResponseAddr_InnerIcmpDifferentId_Invalid)
{
    response.triggerPacket.transportHeader.id--;
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest