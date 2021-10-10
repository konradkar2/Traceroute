#include "IcmpToIcmpBase.hpp"
#include "ResponseValidatorsTests/V4/IpHeaderBuilder.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <gmock/gmock.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;

namespace traceroute::responseValidatorsTests::icmpRequest
{

struct TimeExceededV4 : public IcmpToIcmpBase
{
    const SocketAddress responseSource{"123.123.123.123"};
    const SocketAddress responseDestination = requestSource;
    void SetUp() override
    {
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
        response.ipHeader = ipHeaderBuilder.setDestination(responseDestination)
                                .setSource(responseSource)
                                .setProtocol(responseProtocol)
                                .build();
        response.triggerPacket.ipHeader = ipHeaderBuilder.setProtocol(IPPROTO_ICMP).build();
        response.triggerPacket.transportHeader = icmpProbePacket.GetIcmpHeader();
    }
};

TEST_F(TimeExceededV4, innerIcmpSameIdValid)
{
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->validate(icmpProbePacket, responseSource, responseProtocol, resp, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(TimeExceededV4, innerIcmpDifferentIdInvalid)
{
    response.triggerPacket.transportHeader.id--;
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->validate(icmpProbePacket, responseSource, responseProtocol, resp, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest