#include "../utils/Utils.hpp"
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

struct Icmp4ResponseValidatorTest : public IcmpToIcmpBase
{
    const SocketAddress responseSource{"123.123.123.123"};
    const SocketAddress responseDestination = requestSource;
    const int responseProtocol = IPPROTO_ICMP;
    void SetUp() override
    {
        response.icmpHeader.type = ICMP_TIME_EXCEEDED;
        response.ipv4Header = ipHeaderBuilder.setDestination(responseDestination)
                                  .setSource(responseSource)
                                  .setProtocol(responseProtocol)
                                  .build();
        response.receivedPacketCopy.ipv4Header = ipHeaderBuilder.setProtocol(IPPROTO_ICMP).build();
        response.receivedPacketCopy.protocolHeader = icmpProbePacket.GetIcmpHeader();
    }
};

TEST_F(Icmp4ResponseValidatorTest, timeExceeded_innerIcmpSameIdValid)
{
    const char *respP = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, respP, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(Icmp4ResponseValidatorTest, timeExceeded_innerIcmpDifferentIdInvalid)
{
    response.receivedPacketCopy.protocolHeader.id--;
    const char *respP = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, respP, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest