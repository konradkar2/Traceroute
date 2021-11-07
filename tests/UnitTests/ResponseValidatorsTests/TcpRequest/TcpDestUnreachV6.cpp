#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/utilsV6.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <netinet/icmp6.h>

using namespace traceroute::packet;

namespace traceroute {
struct TcpDestUnreachV6 : public ResponseValidatorTestV6
{
    const int responseProtocol = IPPROTO_ICMPV6;
    const SocketAddress validResponseAddr = requestDestination;

    const TcpPacket request = TcpPacket(requestSource, requestDestination, 80);
    ResponseIcmpToTcp<Ipv6Header> response;
    TcpDestUnreachV6() : ResponseValidatorTestV6(std::make_unique<responseValidators::TcpResponseValidator>())
    {
        response.icmpHeader.type = ICMP6_DST_UNREACH;
        response.triggerPacket.ipHeader.version = 6;
    }
};

TEST_F(TcpDestUnreachV6, valid)
{
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto [resp, responseSize] = responseV6ToPtr(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}

TEST_F(TcpDestUnreachV6, invalidClient)
{
    const SocketAddress invalidResponseAddr{"a:b:c::1"};
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto [resp, responseSize] = responseV6ToPtr(&response);
    bool isValid = validator->validate(request, invalidResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

TEST_F(TcpDestUnreachV6, invalidSeq)
{
    response.triggerPacket.transportHeader.seq = 0;

    auto [resp, responseSize] = responseV6ToPtr(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute