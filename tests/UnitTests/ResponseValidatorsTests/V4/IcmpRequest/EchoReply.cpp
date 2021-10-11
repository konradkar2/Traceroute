
#include "ResponseValidators/V4/Icmp4ResponseValidator.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/V4/IpHeaderVariants.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
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


struct EchoReplyV4 : public ::testing::Test
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
TEST_F(EchoReplyV4, SameIcmpId_Valid)
{
    ResponseIcmpToIcmp<Ipv4Header> response;
    response.ipHeader = createStandardIpHeader();
    response.icmpHeader.id = request.GetIcmpHeader().id;
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(EchoReplyV4, differentIcmpId_Invalid)
{
    ResponseIcmpToIcmp<Ipv4Header> response;
    response.ipHeader = createStandardIpHeader();
    response.icmpHeader.id = request.GetIcmpHeader().id - 1;
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, responseSize);

    EXPECT_FALSE(isValid);
}


TEST_F(EchoReplyV4, CustomIhlSameIcmpId_Valid)
{
    constexpr int ipHeaderOptionsSize = 12;
    ResponseIcmpToIcmp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    response.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
    response.icmpHeader.id = request.GetIcmpHeader().id;
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, responseSize);

    EXPECT_TRUE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest