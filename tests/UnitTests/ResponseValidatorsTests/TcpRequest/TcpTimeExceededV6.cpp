#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/utilsV6.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <gmock/gmock.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <string>
#include <vector>

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::tcpRequest
{
struct TcpTimeExceededV6 : public ResponseValidatorTestV6
{
    const int responseProtocol = IPPROTO_ICMPV6;
    const SocketAddress transitRouter{"1:1:1::5"};
    const TcpPacket request = TcpPacket(requestSource, requestDestination, 80);
    ResponseIcmpToTcp<Ipv6Header> response;
    TcpTimeExceededV6() : ResponseValidatorTestV6(std::make_unique<responseValidators::TcpResponseValidator>())
    {
        response.icmpHeader.type = ICMP6_TIME_EXCEEDED;
        response.triggerPacket.ipHeader.version = 6;
    }
};

TEST_F(TcpTimeExceededV6, valid)
{
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto [resp, responseSize ]= responseV6ToPtr(&response);
    bool isValid = validator->validate(request, transitRouter, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TcpTimeExceededV6, invalidSeq)
{
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq - 1;
    
    auto [resp, responseSize ]= responseV6ToPtr(&response);
    bool isValid = validator->validate(request, transitRouter, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::tcpRequest