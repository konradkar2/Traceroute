#include "ResponseValidatorsTests/IpHeaderVariants.hpp"
#include "ResponseValidatorsTests/Responses.hpp"
#include <ResponseValidatorsTests/ResponseValidatorTest.hpp>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;
namespace traceroute::responseValidatorsTests::icmpRequest {

struct IcmpEchoReplyBaseV4 : public ResponseValidatorTestV4
{
    const int                    responseProtocol  = IPPROTO_ICMP;
    const SocketAddress          validResponseAddr = requestDestination;
    const unique_ptr<IcmpPacket> request           = IcmpPacket::CreateIcmp4Packet(requestSource, requestDestination);
    IcmpEchoReplyBaseV4() : ResponseValidatorTestV4()
    {
        setValidator(std::make_unique<responseValidators::IcmpResponseValidator>(*request));
    }
};

struct IcmpEchoReplyV4 : public IcmpEchoReplyBaseV4
{

    ResponseIcmpToIcmp<Ipv4Header> response;
    IcmpEchoReplyV4()
    {
        response.ipHeader        = createStandardIpHeader();
        response.icmpHeader.type = ICMP_ECHOREPLY;
    }
};

constexpr int ipHeaderOptionsSize = 12;
struct EchoReplyV4CustomIhl : public IcmpEchoReplyBaseV4
{
    ResponseIcmpToIcmp<Ipv4HeaderCustomSize<ipHeaderOptionsSize>> response;
    EchoReplyV4CustomIhl()
    {
        response.ipHeader        = createCustomSizeIpHeader<ipHeaderOptionsSize>();
        response.icmpHeader.type = ICMP_ECHOREPLY;
    }
};

TEST_F(IcmpEchoReplyV4, SameId_Valid)
{
    response.icmpHeader.id = request->GetIcmpHeader().id;

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_TRUE(isValid);
}
TEST_F(IcmpEchoReplyV4, differentId_Invalid)
{
    response.icmpHeader.id = request->GetIcmpHeader().id - 1;

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_FALSE(isValid);
}

TEST_F(EchoReplyV4CustomIhl, sameId_Valid)
{
    response.icmpHeader.id = request->GetIcmpHeader().id;

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_TRUE(isValid);
}

TEST_F(EchoReplyV4CustomIhl, differentId_Invalid)
{
    response.icmpHeader.id = request->GetIcmpHeader().id - 1;

    ResponseInfo respInfo{validResponseAddr, responseProtocol, sizeof(response)};
    auto         resp    = reinterpret_cast<const char *>(&response);
    bool         isValid = validator->validate(respInfo, resp);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest