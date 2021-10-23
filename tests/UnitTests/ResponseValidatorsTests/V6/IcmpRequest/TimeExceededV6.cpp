
#include "ResponseValidators/V6/Icmp6ResponseValidator.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
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

struct TimeExceededV6 : public ResponseValidatorTest
{
    const SocketAddress requestSource{"2a0e:1c80:4:1013::1012"};
    const SocketAddress requestDestination{"5a0e:1b80:4:1013::1015"};
    const SocketAddress transitHost{"6b3e:1b30:4:6043::1011"};
    const IcmpPacket request = IcmpPacket::CreateIcmp6Packet(requestSource, requestDestination);
    ResponseIcmpToIcmp<Ipv6Header> response;
    TimeExceededV6() : ResponseValidatorTest(std::make_unique<responseValidators::v6::Icmp6ResponseValidator>())
    {
        response.icmpHeader.type = ICMP6_TIME_EXCEEDED;
    }
};
TEST_F(TimeExceededV6, sameIdValid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id;

    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);
    resp += sizeof(Ipv6Header);
    bool isValid = validator->validate(request, transitHost, IPPROTO_ICMPV6, resp, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(TimeExceededV6, differentIdInvalid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id - 1;

    const char *resp = reinterpret_cast<const char *>(&response);
    resp += sizeof(Ipv6Header);
    size_t responseSize = sizeof(response);
    bool isValid = validator->validate(request, transitHost, IPPROTO_ICMPV6, resp, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest