#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <Traceroute/ProbeSender.hpp>
#include <Traceroute/DataSenders/TcpDataSender.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>

struct LiveTcpTest_test_dot_com : public ::testing::Test
{
    Traceroute::SocketAddress mDestinationAddr;
    Traceroute::SocketAddress mSource;

    int mFamily;
    int mRetries = 2;
    int mSockDelay = 5;
    std::chrono::milliseconds mTimeoutTotal {100};
    std::chrono::milliseconds mPollTimeout {5};
    Traceroute::ProbeSender *mProbeSender;

    void SetUp() override
    {
        mDestinationAddr = Traceroute::SocketAddress{"69.172.200.235"};
        mSource = Traceroute::SocketAddress("10.8.0.2");
        mFamily = mDestinationAddr.family();
        
        mProbeSender = new Traceroute::ProbeSender(std::make_unique<Traceroute::DataSenders::TcpDataSender>(mSource,mPollTimeout),
				std::make_unique<Traceroute::ResponseValidators::TcpResponseValidator>());
    }
    void TearDown() override
    {
        delete mProbeSender;
    }
};

TEST_F(LiveTcpTest_test_dot_com, GotResponse)
{
    std::vector<Traceroute::ProbeResultContainer> probes;
    for (int ttl = 1; ttl < 32; ++ttl)
    {
        auto packet = Traceroute::PacketBuilder::CreateTcpPacket(mSource, mDestinationAddr, 80);
        auto result = mProbeSender->beginProbing(&packet, ttl, mRetries, mTimeoutTotal);
        probes.push_back(result);
        if (result.GetResponseAddr() == mDestinationAddr)
        {
            break;
        }
    }
    ASSERT_GT(probes.size(), 0);
    auto found = std::find_if(probes.cbegin(), probes.cend(), [addr = mDestinationAddr](const Traceroute::ProbeResultContainer &probe)
                              { return probe.GetResponseAddr() == addr; });
    EXPECT_TRUE(found != probes.end()) << "Didn't found " ;

    std::string result;
    for(const auto & probeResult : probes)
    {
        result+= probeResult.toString() + "\n";
    }
    std::cerr << result;
}
