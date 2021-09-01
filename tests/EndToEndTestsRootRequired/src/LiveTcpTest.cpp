#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/PacketFactory/TcpPacketFactory.hpp>
#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <Traceroute/ProbeSender.hpp>
#include <Traceroute/DataSenders/TcpDataSender.hpp>
#include <gmock/gmock.h>
#include <algorithm>
#include <string>
#include <vector>

using ::testing::Eq;
using ::testing::Gt;
using ::testing::IsTrue;
struct LiveTcpTest_test_dot_com : public ::testing::Test
{

    traceroute::SocketAddress source{"10.8.0.2"};
    traceroute::SocketAddress destination{"69.172.200.235"};
    std::chrono::milliseconds pollTimeout{5};
    std::chrono::milliseconds generalTimeout{200};
    int dport = 80;
    traceroute::ProbeSender probeSender{
					std::make_unique<traceroute::TcpPacketFactory>(source,destination,dport),
					std::make_unique<traceroute::DataSenders::TcpDataSender>(source,pollTimeout),
					std::make_unique<traceroute::responseValidators::TcpResponseValidator>()};
};

TEST_F(LiveTcpTest_test_dot_com, GotResponse)
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
