
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "ResponseValidators/V4/Icmp4ResponseValidator.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <netinet/ip_icmp.h>
#include "../Utils.hpp"

using namespace traceroute::packet;
namespace traceroute::responseValidatorsTests::icmpRequest
{
    namespace icmpRequest
    {

        struct Icmp4EchoReply : public ::testing::Test
        {
            std::unique_ptr<traceroute::IValidateResponse> validator = std::make_unique<responseValidators::v4::Icmp4ResponseValidator>();
            const SocketAddress requestSource{"192.168.1.1"};
            const SocketAddress requestDestination{"8.8.8.8"};
            const IcmpPacket icmpProbePacket = IcmpPacket::CreateIcmp4Packet(requestSource, requestDestination);
            const SocketAddress responseSource = requestDestination;
            const SocketAddress responseDestination = requestSource;
            ResponseIcmpToIcmp response;
            const int responseProtocol = IPPROTO_ICMP;
            const int responseIhl = 5;
            const char *dataPtr;
            void SetUp() override
            {
                response.icmpHeader.type = ICMP_ECHOREPLY;
                response.receivedPacketCopy.ipv4Header.ihl = 5;
                response.receivedPacketCopy.ipv4Header.version = 4;
                fillIPv4Header(responseSource, responseDestination, responseIhl, responseProtocol, &response.ipv4Header);
            }
        };
        TEST_F(Icmp4EchoReply, hitTargetAndItRespondedWithIcmpEchoReplyWithIdenticalSequence)
        {

            response.icmpHeader.sequence = icmpProbePacket.GetIcmpHeader().sequence;
            const char *dataPtr = reinterpret_cast<const char *>(&response);
            size_t responseSize = sizeof(response);

            auto result = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, dataPtr, responseSize);

            EXPECT_TRUE(result);
        }
        TEST_F(Icmp4EchoReply, hitTargetAndItRespondedWithIcmpEchoReplyWithDifferentSequence)
        {
            response.icmpHeader.sequence = icmpProbePacket.GetIcmpHeader().sequence - 1;
            const char *dataPtr = reinterpret_cast<const char *>(&response);
            size_t responseSize = sizeof(response);

            auto result = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, dataPtr, responseSize);

            EXPECT_FALSE(result);
        }
    }
}