
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "ResponseValidators/V4/Icmp4ResponseValidator.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>
#include "../Utils.hpp"

namespace ResponseValidatorsTests
{
    namespace IcmpRequest
    {
        
        struct Icmp4EchoReply : public ::testing::Test
        {
            std::unique_ptr<Traceroute::IValidateResponse> validator = std::make_unique<Traceroute::ResponseValidators::V4::Icmp4ResponseValidator>();
            const Traceroute::SocketAddress requestSource{"192.168.1.1"};
            const Traceroute::SocketAddress requestDestination{"8.8.8.8"};
            const Traceroute::IcmpPacket icmpProbePacket = Traceroute::PacketBuilder::CreateIcmpPacket(requestSource, requestDestination);
            const Traceroute::SocketAddress responseSource = requestDestination;
            const Traceroute::SocketAddress responseDestination = requestSource;
            ResponseIcmp<Traceroute::IcmpHeader> response;
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
            response.icmpHeader.sequence = icmpProbePacket.GetIcmpHeader().sequence -1;
            const char *dataPtr = reinterpret_cast<const char *>(&response);
            size_t responseSize = sizeof(response);

            auto result = validator->isResponseValid(icmpProbePacket, responseSource, responseProtocol, dataPtr, responseSize);

            EXPECT_FALSE(result);
        }
    }
}