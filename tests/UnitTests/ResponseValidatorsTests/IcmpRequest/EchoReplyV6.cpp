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
namespace traceroute::responseValidatorsTests::icmpRequest {

struct IcmpEchoReplyV6 : ResponseValidatorTestV6
{
    const SocketAddress invalidResponseAddress{"6b3e:1b30:4:6043::1011"};

    const unique_ptr<IcmpPacket>   request          = IcmpPacket::CreateIcmp6Packet(requestSource, requestDestination);
    const int                      responseProtocol = IPPROTO_ICMPV6;
    ResponseIcmpToIcmp<Ipv6Header> response;
    IcmpEchoReplyV6() : ResponseValidatorTestV6()
    {
        setValidator(std::make_unique<responseValidators::IcmpResponseValidator>(*request));
        response.icmpHeader.type                = ICMP6_ECHO_REPLY;
        response.triggerPacket.ipHeader.version = 6;
    }
};
TEST_F(IcmpEchoReplyV6, sameIdValid)
{
    response.icmpHeader.id = request->GetIcmpHeader().id;

    auto [resp, responseSize] = responseV6ToPtr(&response);
    ResponseInfo respInfo{requestDestination, responseProtocol, responseSize};
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_TRUE(isValid);
}

TEST_F(IcmpEchoReplyV6, InvalidAddressSameIdInvalid)
{
    response.icmpHeader.id = request->GetIcmpHeader().id;

    auto [resp, responseSize] = responseV6ToPtr(&response);
    ResponseInfo respInfo{invalidResponseAddress, responseProtocol, responseSize};
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_FALSE(isValid);
}

TEST_F(IcmpEchoReplyV6, differentIdInvalid)
{
    response.icmpHeader.id   = request->GetIcmpHeader().id - 1;
    response.icmpHeader.type = ICMP6_ECHO_REPLY;

    auto [resp, responseSize] = responseV6ToPtr(&response);
    ResponseInfo respInfo{requestDestination, responseProtocol, responseSize};
    bool         isValid = validator->isValid(respInfo, resp);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest