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
    std::string mDestinationText;
    Traceroute::SocketAddress mDestinationAddr;
    Traceroute::SocketAddress mSource;

    int mFamily;
    int mRetries = 2;
    int mSockDelay = 5;
    std::chrono::milliseconds mTimeout{100};
    Traceroute::ProbeSender *mProbeSender;

    void SetUp() override
    {
        mDestinationText = "69.172.200.235";
        mDestinationAddr = Traceroute::SocketAddress{mDestinationText};
        mSource = Traceroute::SocketAddress("192.168.197.250");
        mFamily = mDestinationAddr.getFamily();
        
        mProbeSender = new Traceroute::ProbeSender(std::make_unique<Traceroute::DataSenders::TcpDataSender>(mFamily,mSource,mRetries),
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
        auto result = mProbeSender->beginProbing(&packet, ttl, mRetries, mTimeout);
        probes.push_back(result);
        if (result.GetResponseAddr() == mDestinationText)
        {
            break;
        }
    }
    ASSERT_GT(probes.size(), 0);
    auto found = std::find_if(probes.cbegin(), probes.cend(), [dText = mDestinationText](const Traceroute::ProbeResultContainer &probe)
                              { return probe.GetResponseAddr() == dText; });
    EXPECT_TRUE(found != probes.end()) << "Didn't found " << mDestinationText;

    std::string result;
    for(const auto & probeResult : probes)
    {
        result+= probeResult.toString() + "\n";
    }
    std::cerr << result;
}
