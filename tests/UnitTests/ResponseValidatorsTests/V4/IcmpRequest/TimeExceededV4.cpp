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
    const int responseProtocol = IPPROTO_ICMP;
    const SocketAddress ArbitraryDestAddr{"192.51.100.1"};
    std::unique_ptr<traceroute::IValidateResponse> validator =
        std::make_unique<responseValidators::v4::Icmp4ResponseValidator>();
    const SocketAddress requestSource{"192.0.2.1"};
    const SocketAddress validResponseAddr = requestSource;
    const IcmpPacket request = IcmpPacket::CreateIcmp4Packet(requestSource, ArbitraryDestAddr);

    void SetUp() override
    {
    }
};

TEST_F(TimeExceededV4, ProperResponseAddr_InnerIcmpSameId_Valid)
{
    ResponseIcmpToIcmp<Ipv4Header> response;
    response.ipHeader = createStandardIpHeader();
    response.icmpHeader.type = ICMP_TIME_EXCEEDED;
    
    response.triggerPacket.ipHeader = createStandardIpHeader();
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TimeExceededV4, ProperResponseAddr_InnerIcmpDifferentId_Invalid)
{
    ResponseIcmpToIcmp<Ipv4Header> response;
    response.icmpHeader.type = ICMP_TIME_EXCEEDED;
    response.ipHeader = createStandardIpHeader();
    response.triggerPacket.ipHeader = createStandardIpHeader();
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id - 1;

    auto resp  = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest