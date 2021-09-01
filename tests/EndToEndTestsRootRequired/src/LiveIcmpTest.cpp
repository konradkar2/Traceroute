#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/PacketFactory/IcmpPacketFactory.hpp>
#include <Traceroute/DataSenders/IcmpDataSender.hpp>
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include <Traceroute/ProbeSender.hpp>
#include <gmock/gmock.h>
#include <algorithm>
#include <string>
#include <vector>
using ::testing::Eq;
using ::testing::Gt;
using ::testing::IsTrue;
struct LiveIcmpTest_8888 : public ::testing::Test
{

    traceroute::SocketAddress source{"10.8.0.2"};
    traceroute::SocketAddress destination{"8.8.8.8"};
    std::chrono::milliseconds pollTimeout{5};
    std::chrono::milliseconds generalTimeout{200};
    traceroute::ProbeSender probeSender{
					std::make_unique<traceroute::IcmpPacketFactory>(source,destination),
					std::make_unique<traceroute::DataSenders::IcmpDataSender>(source,pollTimeout),
					std::make_unique<traceroute::responseValidators::IcmpResponseValidator>()};
};

TEST_F(LiveIcmpTest_8888, GotResponseFrom8888)
{
    int startTtl = 1;
    int endTtl = 15;
    int retries = 2;

    const auto probes = probeSender.beginProbing(startTtl,endTtl,retries,generalTimeout);
    
    ASSERT_THAT(probes.size(),Gt(0));
    bool isDestinationFound = std::any_of(probes.cbegin(), probes.cend(), [addr = destination](const traceroute::ProbeResultContainer &probe)
                              { return probe.GetResponseAddr() == addr; });
    EXPECT_THAT(isDestinationFound,IsTrue);

    for(auto & result : probes)
    {
        std::cerr<<result.toString()<<std::endl;
    }
}
