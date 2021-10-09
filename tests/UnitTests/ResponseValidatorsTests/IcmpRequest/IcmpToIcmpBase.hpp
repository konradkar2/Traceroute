#pragma once
#include "../utils/IpHeaderBuilder.hpp"
#include "../utils/Utils.hpp"
#include "ResponseValidators/V4/Icmp4ResponseValidator.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <gtest/gtest.h>
#include <netinet/ip_icmp.h>
#include <string>
#include <vector>

using namespace traceroute::packet;
namespace traceroute
{

struct IcmpToIcmpBase : public ::testing::Test
{
    std::unique_ptr<traceroute::IValidateResponse> validator =
        std::make_unique<responseValidators::v4::Icmp4ResponseValidator>();
    const SocketAddress requestSource{"192.168.1.1"};
    const SocketAddress requestDestination{"8.8.8.8"};
    const IcmpPacket icmpProbePacket = IcmpPacket::CreateIcmp4Packet(requestSource, requestDestination);
    ResponseIcmpToIcmp response;
    IpHeaderBuilder ipHeaderBuilder;
};

} // namespace traceroute