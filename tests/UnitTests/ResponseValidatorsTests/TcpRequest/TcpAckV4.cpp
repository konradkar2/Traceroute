#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include "ResponseValidatorsTests/Responses.hpp"
#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/TcpPacket.hpp>
#include <cstdint>
#include <gmock/gmock.h>
#include <netinet/in.h>
#include <netinet/icmp6.h>
#include <string>
#include <vector>
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::tcpRequest
{

namespace{
    uint32_t getValidReponseAckSeq(const TcpHeader & requestN)
    {
        uint32_t ackSeq = ntohl(requestN.seq);
        ackSeq += 1;
        return htonl(ackSeq);
    }
}

struct TcpAckBase : public ResponseValidatorTestV4
{
    const int responseProtocol = IPPROTO_TCP;
    const SocketAddress validResponseAddr = requestDestination;

    const TcpPacket request = TcpPacket(requestSource,requestDestination,80);
    TcpAckBase() : ResponseValidatorTestV4(std::make_unique<responseValidators::TcpResponseValidator>())
    {
    }
};

struct TcpAckV4 : public TcpAckBase
{

    ResponseTcpToTcp<Ipv4Header> response;
    TcpAckV4()
    {
        response.ipHeader = createStandardIpHeader();
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct TcpAckV4CustomIhl : public TcpAckBase
{
    ResponseTcpToTcp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    TcpAckV4CustomIhl()
    {
        response.ipHeader = createCustomSizeIpHeader<ipHeaderOptionsSize>();
    }
};

TEST_F(TcpAckV4, sameAckSeq_Valid)
{
    response.tcpHeader.ack_seq = getValidReponseAckSeq(request.getTcpHeader());

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TcpAckV4, differenAckSeq_Invalid)
{
    response.tcpHeader.ack_seq = 0;
    auto resp  = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

TEST_F(TcpAckV4, differentClient_Invalid)
{
    const SocketAddress invalidResponseAddr{"1.1.1.1"};
    response.tcpHeader.ack_seq = getValidReponseAckSeq(request.getTcpHeader());

    auto resp  = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, invalidResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

TEST_F(TcpAckV4CustomIhl, sameAckSeq_Valid)
{
    response.tcpHeader.ack_seq = getValidReponseAckSeq(request.getTcpHeader());

    auto resp = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_TRUE(isValid);
}
TEST_F(TcpAckV4CustomIhl, differenAcktSeq_Invalid)
{
    response.tcpHeader.ack_seq = 0;
    
    auto resp  = reinterpret_cast<const char *>(&response);
    bool isValid = validator->validate(request, validResponseAddr, responseProtocol, resp, sizeof(response));

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest