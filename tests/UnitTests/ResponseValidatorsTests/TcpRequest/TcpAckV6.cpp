#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <cstdint>
#include <gmock/gmock.h>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include "ResponseValidatorsTests/utilsV6.hpp"

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::tcpRequest
{

namespace
{
uint32_t getValidReponseAckSeq(const TcpHeader &requestN)
{
    uint32_t ackSeq = ntohl(requestN.seq);
    ackSeq += 1;
    return htonl(ackSeq);
}
} // namespace

struct TcpAckV6 : public ResponseValidatorTestV6
{
    const int responseProtocol = IPPROTO_TCP;
    const SocketAddress validResponseAddr = requestDestination;

    const TcpPacket request = TcpPacket(requestSource, requestDestination, 80);
    ResponseTcpToTcp<Ipv6Header> response;
    TcpAckV6() : ResponseValidatorTestV6(std::make_unique<responseValidators::TcpResponseValidator>())
    {
    }
};

TEST_F(TcpAckV6, valid)
{
    response.tcpHeader.ack_seq = getValidReponseAckSeq(request.getTcpHeader());

    auto [resp, responseSize ]= responseV6ToPtr(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TcpAckV6, invalidAckSeq)
{
    response.tcpHeader.ack_seq = 0;

    auto [resp, responseSize ]= responseV6ToPtr(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

TEST_F(TcpAckV6, invalidClient)
{
    const SocketAddress invalidResponseAddr{"::1"};
    response.tcpHeader.ack_seq = getValidReponseAckSeq(request.getTcpHeader());

    auto [resp, responseSize ]= responseV6ToPtr(&response);
    bool isValid = validator->validate(request, invalidResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::tcpRequest