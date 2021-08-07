#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/Tcp/TcpResponseValidator.hpp>
#include <Traceroute/ProbeSender.hpp>
#include <Traceroute/Tcp/TcpDataSender.hpp>
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
    Traceroute::Tcp::TcpDataSender *mDataSender;
    Traceroute::Tcp::TcpResponseValidator *mResponseValidator;
    void SetUp() override
    {
        mDestinationText = "69.172.200.235";
        mDestinationAddr = Traceroute::SocketAddress{mDestinationText};
        mSource = Traceroute::SocketAddress("192.168.132.129");
        mFamily = mDestinationAddr.getFamily();
        mDataSender = new Traceroute::Tcp::TcpDataSender(mFamily, mSource, mSockDelay);
        mResponseValidator = new Traceroute::Tcp::TcpResponseValidator;
        mProbeSender = new Traceroute::ProbeSender(mDataSender, mResponseValidator);
    }
    void TearDown() override
    {
        delete mDataSender;
        delete mResponseValidator;
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
}
