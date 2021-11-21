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

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::tcpRequest {

namespace {
uint32_t getValidReponseAckSeq(const TcpHeader &requestN)
{
    uint32_t ackSeq = ntohl(requestN.seq);
    ackSeq += 1;
    return htonl(ackSeq);
}
} // namespace

struct TcpAckBase : public ResponseValidatorTestV4
{
    const int           responseProtocol  = IPPROTO_TCP;
    const SocketAddress validResponseAddr = requestDestination;

    const TcpPacket request = TcpPacket(requestSource, requestDestination, 80);
    TcpAckBase()
    {
        setValidator(std::make_unique<responseValidators::TcpResponseValidator>(request));
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

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_TRUE(isValid);
}
TEST_F(TcpAckV4, differenAckSeq_Invalid)
{
    response.tcpHeader.ack_seq = 0;
    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_FALSE(isValid);
}

TEST_F(TcpAckV4, differentClient_Invalid)
{
    const SocketAddress invalidResponseAddr{"1.1.1.1"};
    response.tcpHeader.ack_seq = getValidReponseAckSeq(request.getTcpHeader());

    ResponseInfo respInfo{invalidResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_FALSE(isValid);
}

TEST_F(TcpAckV4CustomIhl, sameAckSeq_Valid)
{
    response.tcpHeader.ack_seq = getValidReponseAckSeq(request.getTcpHeader());

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_TRUE(isValid);
}
TEST_F(TcpAckV4CustomIhl, differenAcktSeq_Invalid)
{
    response.tcpHeader.ack_seq = 0;

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::tcpRequest