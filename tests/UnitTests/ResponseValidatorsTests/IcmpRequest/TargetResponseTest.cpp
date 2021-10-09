#include "../utils/Utils.hpp"
#include "IcmpToIcmpBase.hpp"
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

struct Icmp4ResponseValidatorTest : public IcmpToIcmpBase
{
    const SocketAddress responseSource = requestDestination;
    const SocketAddress responseDestination = requestSource;
    const int responseProtocol = IPPROTO_ICMP;
    void SetUp() override
    {
        response.icmpHeader.type = ICMP_ECHOREPLY;
        response.ipv4Header = ipHeaderBuilder.setDestination(responseDestination)
                                  .setSource(responseSource)
                                  .setProtocol(responseProtocol)
                                  .build();
    }
};
TEST_F(Icmp4ResponseValidatorTest, echoReply_identicalSequenceValid)
{
    response.icmpHeader.id = icmpProbePacket.GetIcmpHeader().id;
    const char *dataPtr = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, dataPtr, responseSize);

    EXPECT_TRUE(isValid);
}
TEST_F(Icmp4ResponseValidatorTest, echoReply_differentSequenceInvalid)
{
    response.icmpHeader.sequence = icmpProbePacket.GetIcmpHeader().sequence - 1;
    const char *dataPtr = reinterpret_cast<const char *>(&response);
    size_t responseSize = sizeof(response);

    bool isValid = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, dataPtr, responseSize);

    EXPECT_FALSE(isValid);
}

} // namespace traceroute::responseValidatorsTests::icmpRequest