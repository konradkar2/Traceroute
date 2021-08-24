
#include <gmock/gmock.h>
#include <string>
#include <vector>
#include "ResponseValidators/V4/Icmp4ResponseValidator.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>
#include "../Utils.hpp"

using ::testing::Eq;
namespace ResponseValidatorsTests
{
    namespace IcmpRequest
    {
        struct Icmp4TimeExceed : public ::testing::Test
        {
            std::unique_ptr<Traceroute::IValidateResponse> validator = std::make_unique<Traceroute::ResponseValidators::V4::Icmp4ResponseValidator>();
            const Traceroute::SocketAddress requestSource{"192.168.1.1"};
            const Traceroute::SocketAddress requestDestination{"8.8.8.8"};
            const Traceroute::IcmpPacket icmpProbePacket = Traceroute::PacketBuilder::CreateIcmpPacket(requestSource, requestDestination);
            const Traceroute::SocketAddress responseSourceTransitRouter{"22.22.22.22"};
            const Traceroute::SocketAddress responseDestination = requestSource;
            ResponseIcmpToIcmp response;
            const int protocol = IPPROTO_ICMP;
            const int ihl = 5;          
            const char *dataPtr;
            void SetUp() override
            {
                fillIPv4Header(responseSourceTransitRouter, responseDestination, ihl, protocol, &response.ipv4Header);
                response.icmpHeader.type = ICMP_TIME_EXCEEDED;
                response.receivedPacketCopy.ipv4Header.ihl = 5;
                response.receivedPacketCopy.ipv4Header.version = 4;
            }
        };

        TEST_F(Icmp4TimeExceed, hitTransitRouterAndItRespondedWithIcmpTimeExceededContainingProperIcmpId)
        {
            response.receivedPacketCopy.protocolHeader.id = icmpProbePacket.GetIcmpHeader().id;
            const char *dataPtr = reinterpret_cast<const char *>(&response);
            size_t responseSize = sizeof(response);

            bool isResponseValid = validator->isResponseValid(icmpProbePacket, responseSourceTransitRouter, protocol, dataPtr, responseSize);

            EXPECT_THAT(isResponseValid, Eq(true));
        }
        TEST_F(Icmp4TimeExceed, hitTransitRouterAndItRespondedWithIcmpTimeExceededNOTContainingProperIcmpId)
        {
            response.receivedPacketCopy.protocolHeader.id = icmpProbePacket.GetIcmpHeader().id - 1;
            const char *dataPtr = reinterpret_cast<const char *>(&response);
            size_t responseSize = sizeof(response);

            bool isResponseValid = validator->isResponseValid(icmpProbePacket, responseSourceTransitRouter, protocol, dataPtr, responseSize);

            EXPECT_THAT(isResponseValid, Eq(false));
        }
      
    }
}