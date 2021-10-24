#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
#include <gmock/gmock.h>
#include <netinet/in.h>
#include <netinet/icmp6.h>
#include <string>
#include <vector>
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::tcpRequest
{
struct TimeExceededBase : public ResponseValidatorTestV4
{
    const int responseProtocol = IPPROTO_ICMP;
    const SocketAddress transitRouter{"23.42.42.42"};

    const TcpPacket request = TcpPacket(requestSource,requestDestination,80);
    TimeExceededBase() : ResponseValidatorTestV4(std::make_unique<responseValidators::TcpResponseValidator>())
    {
    }
};

struct TcpTimeExceededV4 : public TimeExceededBase
{

    ResponseIcmpToTcp<Ipv4Header> response;
    TcpTimeExceededV4()
    {
        response.ipHeader = createStandardIpHeader();
        response.triggerPacket.ipHeader = createStandardIpHeader();
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct TcpTimeExceededV4CustomIhl : public TimeExceededBase
{
    ResponseIcmpToTcp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    TcpTimeExceededV4CustomIhl()
    {
        response.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.triggerPacket.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
    }
};

TEST_F(TcpTimeExceededV4, sameSeq_Valid)
{
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, transitRouter, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TcpTimeExceededV4, differentSeq_Invalid)
{
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq -1;
    auto resp  = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, transitRouter, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

TEST_F(TcpTimeExceededV4CustomIhl, sameId_Valid)
{
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq;

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, transitRouter, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TcpTimeExceededV4CustomIhl, differentId_Invalid)
{
    response.triggerPacket.transportHeader.seq = request.getTcpHeader().seq -1;

    auto resp  = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, transitRouter, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest