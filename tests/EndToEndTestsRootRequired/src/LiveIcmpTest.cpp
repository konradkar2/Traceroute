#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/DataSenders/IcmpDataSender.hpp>
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include <Traceroute/ProbeSender.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>

struct LiveIcmpTest_8888 : public ::testing::Test
{

    Traceroute::SocketAddress mDestinationAddr;
    Traceroute::SocketAddress mSource;

    int mFamily;
    int mRetries = 2;
    int mSockDelay = 5;
    std::chrono::milliseconds mTimeoutTotal{100};
    std::chrono::milliseconds mPollTimeout{5};
    Traceroute::ProbeSender *mProbeSender;
    void SetUp() override
    {
        mDestinationAddr = Traceroute::SocketAddress{"8.8.8.8"};
        mSource = Traceroute::SocketAddress("10.8.0.2");
        mFamily = mDestinationAddr.family();
        mProbeSender = new Traceroute::ProbeSender(std::make_unique<Traceroute::DataSenders::IcmpDataSender>(mSource, mPollTimeout),
                                                   std::make_unique<Traceroute::ResponseValidators::IcmpResponseValidator>());
    }
    void TearDown() override
    {
        delete mProbeSender;
    }
};

TEST_F(LiveIcmpTest_8888, GotResponseFrom8888)
{
    std::vector<Traceroute::ProbeResultContainer> probes;
    for (int ttl = 1; ttl < 32; ++ttl)
    {
        auto packet = Traceroute::PacketBuilder::CreateIcmpPacket(mSource, mDestinationAddr);
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
    EXPECT_TRUE(found != probes.end()) << "Didn't found ";

    std::string result;
    for (const auto &probeResult : probes)
    {
        result += probeResult.toString() + "\n";
    }
    std::cerr << result;
}
