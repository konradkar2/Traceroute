#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/IcmpProbeSender.hpp>
#include <Traceroute/IcmpDataSender.hpp>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>


struct LiveIcmpTest_8888 : public ::testing::Test
{
    std::string mDestinationText;
    Traceroute::SocketAddress mDestinationAddr;
    Traceroute::SocketAddress mSource;

    int mFamily;
    int mRetries = 3;
    std::chrono::milliseconds mTimeout {200};
    Traceroute::IcmpProbeSender *  mIcmpProbeSender;
    void SetUp() override
    {
        mDestinationText = "8.8.8.8";
        mDestinationAddr = Traceroute::SocketAddress{mDestinationText};
        mSource = Traceroute::SocketAddress("192.168.242.129");
        mFamily = mDestinationAddr.getFamily();       
        mIcmpProbeSender = new Traceroute::IcmpProbeSender(new Traceroute::IcmpDataSender(mFamily, mSource, mRetries));
    }
    void TearDown() override
    {
        delete mIcmpProbeSender;
    }
    
};

TEST_F(LiveIcmpTest_8888, GotResponseFrom8888)
{
    std::vector<Traceroute::ProbeResultContainer> probes;
    for (int ttl = 1; ttl < 32; ++ttl)
    {
        auto packet = Traceroute::PacketBuilder::CreateIcmpPacket(mSource, mDestinationAddr);
        auto result = mIcmpProbeSender->SendProbe(&packet, ttl, mRetries, mTimeout);
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
    EXPECT_TRUE(found != probes.end()) << "Didn't found ";
    
}
