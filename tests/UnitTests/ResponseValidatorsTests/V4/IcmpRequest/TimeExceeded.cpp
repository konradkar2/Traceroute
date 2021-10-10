#include "ResponseValidatorsTests/V4/IpHeaderBuilder.hpp"
#include "IcmpToIcmpBase.hpp"
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

struct Icmp4TimeExceed: public IcmpToIcmpBase
{
    const SocketAddress responseSource{"123.123.123.123"};
    const SocketAddress responseDestination = requestSource;
    void SetUp() override
    {
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
        response.ipv4Header = ipHeaderBuilder.setDestination(responseDestination)
                                  .setSource(responseSource)
                                  .setProtocol(responseProtocol)
                                  .build();
        response.triggerPacket.ipHeader = ipHeaderBuilder.setProtocol(IPPROTO_ICMP).build();
        response.triggerPacket.transportHeader = icmpProbePacket.GetIcmpHeader();
    }
};

TEST_F(Icmp4TimeExceed, innerIcmpSameIdValid)
{
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, resp, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(Icmp4TimeExceed, innerIcmpDifferentIdInvalid)
{
    response.triggerPacket.transportHeader.id--;
    const char *resp = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, resp, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest