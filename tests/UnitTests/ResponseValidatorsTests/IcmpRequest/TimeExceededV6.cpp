#include "ResponseValidatorsTests/utilsV6.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <ResponseValidatorsTests/Responses.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include <gtest/gtest.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;
namespace traceroute {

struct IcmpTimeExceededV6 : public ResponseValidatorTestV6
{
    const SocketAddress transitHost{"6b3e:1b30:4:6043::1011"};

    const IcmpPacket request = IcmpPacket::CreateIcmp6Packet(requestSource, requestDestination);
    ResponseIcmpToIcmp<Ipv6Header> response;
    IcmpTimeExceededV6() : ResponseValidatorTestV6(std::make_unique<responseValidators::IcmpResponseValidator>())
    {
        response.icmpHeader.type = ICMP6_TIME_EXCEEDED;
        response.triggerPacket.ipHeader.version = 6;
    }
};
TEST_F(IcmpTimeExceededV6, sameIdValid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id;

    auto [resp, responseSize] = responseV6ToPtr(&response);
    bool isValid = validator->validate(request, transitHost, IPPROTO_ICMPV6, resp, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(IcmpTimeExceededV6, differentIdInvalid)
{
    response.triggerPacket.transportHeader.id = request.GetIcmpHeader().id - 1;

    auto [resp, responseSize] = responseV6ToPtr(&response);
    bool isValid = validator->validate(request, transitHost, IPPROTO_ICMPV6, resp, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute