#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/TcpProbeSender.hpp>
#include <Traceroute/TcpDataSender.hpp>
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
    int mRetries = 3;
    std::chrono::milliseconds mTimeout {200};
    Traceroute::TcpProbeSender *  mTcpProbSender;
    void SetUp() override
    {
        mDestinationText = "69.172.200.235";
        mDestinationAddr = Traceroute::SocketAddress{mDestinationText};
        mSource = Traceroute::SocketAddress("192.168.242.129");
        mFamily = mDestinationAddr.getFamily();       
        mTcpProbSender = new Traceroute::TcpProbeSender(new Traceroute::TcpDataSender(mFamily, mSource, mRetries));
    }
    void TearDown() override
    {
        delete mTcpProbSender;
    }
    
};

TEST_F(LiveTcpTest_test_dot_com, GotResponseFrom8888)
{
    std::vector<Traceroute::ProbeResultContainer> probes;
    for (int ttl = 1; ttl < 32; ++ttl)
    {
        auto packet = Traceroute::PacketBuilder::CreateTcpPacket(mSource, mDestinationAddr,80);
        auto result = mTcpProbSender->SendProbe(&packet, ttl, mRetries, mTimeout);
        probes.push_back(result);
        if (result.GetResponseAddr() == mDestinationText)
        {
            break;
        }
    }
    ASSERT_GT(probes.size(),0);
    auto found = std::find_if(probes.cbegin(),probes.cend(),[dText=mDestinationText](const Traceroute::ProbeResultContainer & probe){
        return probe.GetResponseAddr() == dText;
    });
    EXPECT_TRUE(found != probes.end()) << "Didn't found " <<  mDestinationText;
    
}
