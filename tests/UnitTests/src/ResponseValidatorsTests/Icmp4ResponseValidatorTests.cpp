
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "Icmp/V4/Icmp4ResponseValidator.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>
#include "Utils.hpp"

namespace ResponseValidatorsTests
{
    template <typename RequestProtocol>
    struct ResponseIcmp
    {
        struct ReceivedPacketCopy
        {
            Traceroute::Ipv4Header ipv4Header;
            RequestProtocol protocolHeader;
        };
        Traceroute::Ipv4Header ipv4Header;
        Traceroute::IcmpHeader icmpHeader;
        ReceivedPacketCopy receivedPacketCopy;
    };
    struct Icmp4TimeExceed : public ::testing::Test
    {
        Traceroute::IValidateResponse &validator = Traceroute::Icmp::V4::Icmp4ResponseValidator::instance();
        const Traceroute::SocketAddress requestSource{"192.168.1.1"};
        const Traceroute::SocketAddress requestDestination{"8.8.8.8"};
        const Traceroute::IcmpPacket icmpProbePacket = Traceroute::PacketBuilder::CreateIcmpPacket(requestSource, requestDestination);
        const Traceroute::SocketAddress responseSourceTransitRouter{"22.22.22.22"};
        const Traceroute::SocketAddress responseDestination = requestSource;
        ResponseIcmp<Traceroute::IcmpHeader> response;
        const int protocol = IPPROTO_ICMP;
        const int ihl = 5;
        const int ttl = 64;
        const char *dataPtr;
        void SetUp() override
        {
            fillIPv4Header(responseSourceTransitRouter, responseDestination, ihl, protocol, ttl, &response.ipv4Header);
            response.icmpHeader.type = ICMP_TIME_EXCEEDED;
            response.receivedPacketCopy.ipv4Header.ihl = 5;
        }
    };

    TEST_F(Icmp4TimeExceed, hitTransitRouterAndItRespondedWithIcmpTimeExceededContainingProperIcmpIdTest)
    {
        response.receivedPacketCopy.protocolHeader.id = icmpProbePacket.GetIcmpHeader().id;
        const char *dataPtr = reinterpret_cast<const char *>(&response);
        size_t responseSize = sizeof(response);
       
        auto result = validator.isResponseValid(icmpProbePacket, responseSourceTransitRouter, protocol, dataPtr, responseSize);

        EXPECT_TRUE(result);
    }
    TEST_F(Icmp4TimeExceed, hitTransitRouterAndItRespondedWithIcmpTimeExceededNOTContainingProperIcmpIdTest)
    {
        response.receivedPacketCopy.protocolHeader.id = icmpProbePacket.GetIcmpHeader().id -1 ;
        const char *dataPtr = reinterpret_cast<const char *>(&response);
        size_t responseSize = sizeof(response);
        
        auto result = validator.isResponseValid(icmpProbePacket, responseSourceTransitRouter, protocol, dataPtr, responseSize);

        EXPECT_FALSE(result);
    }
}