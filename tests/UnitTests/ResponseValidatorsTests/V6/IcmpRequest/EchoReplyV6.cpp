
#include "ResponseValidators/V6/Icmp6ResponseValidator.hpp"
#include <ResponseValidatorsTests/Responses.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <gtest/gtest.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;
namespace traceroute::responseValidatorsTests::icmpRequest
{

struct Ipv6Header
{
    char content[40];
};

struct EchoReplyV6 : public ::testing::Test
{
    std::unique_ptr<traceroute::IValidateResponse> validator =
        std::make_unique<responseValidators::v6::Icmp6ResponseValidator>();
    const SocketAddress requestSource{"2a0e:1c80:4:1013::1012"};
    const SocketAddress requestDestination{"5a0e:1b80:4:1013::1015"};
    const IcmpPacket request = IcmpPacket::CreateIcmp6Packet(requestSource, requestDestination);
    void SetUp() override
    {
    }
};
TEST_F(EchoReplyV6, sameIdValid)
{
    ResponseIcmpToIcmp<Ipv6Header> response;
    response.icmpHeader.id = request.GetIcmpHeader().id;
    response.icmpHeader.type = ICMP6_ECHO_REPLY;

    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);
    resp += sizeof(Ipv6Header);
    bool isValid = validator->validate(request, requestDestination, IPPROTO_ICMPV6, resp, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(EchoReplyV6, differentIdInvalid)
{
    ResponseIcmpToIcmp<Ipv6Header> response;
    response.icmpHeader.id = request.GetIcmpHeader().id -1;
    response.icmpHeader.type = ICMP6_ECHO_REPLY;
    
    const char *resp = reinterpret_cast<const char *>(&response);
    resp += sizeof(Ipv6Header);
    size_t responseSize = sizeof(response);
    bool isValid = validator->validate(request, requestDestination, IPPROTO_ICMPV6, resp, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest