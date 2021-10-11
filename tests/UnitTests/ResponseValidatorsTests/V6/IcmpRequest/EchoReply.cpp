
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <gtest/gtest.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;
namespace traceroute::responseValidatorsTests::icmpRequest
{

struct Ipv6Header
{
    char content [40];
};


struct EchoReplyV6 : public IcmpToIcmpBase
{
    const SocketAddress responseSource = requestDestination;
    const SocketAddress responseDestination = requestSource;
    void SetUp() override
    {
        response.icmpHeader.type = ICMP_ECHOREPLY;
        response.ipHeader = ipHeaderBuilder.setDestination(responseDestination)
                                .setSource(responseSource)
                                .setProtocol(responseProtocol)
                                .build();
    }
};
TEST_F(EchoReplyV4, sameIdValid)
{
    response.icmpHeader.id = icmpProbePacket.GetIcmpHeader().id;
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->validate(icmpProbePacket, responseSource, responseProtocol, resp, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(EchoReplyV4, differentIdInvalid)
{
    response.icmpHeader.id = icmpProbePacket.GetIcmpHeader().id - 1;
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->validate(icmpProbePacket, responseSource, responseProtocol, resp, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest