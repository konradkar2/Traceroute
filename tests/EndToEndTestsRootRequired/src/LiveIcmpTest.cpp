#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/PacketBuilder.hpp>
#include <Traceroute/Icmp/IcmpDataSender.hpp>
#include <Traceroute/Icmp/IcmpResponseValidator.hpp>
#include <Traceroute/ProbeSender.hpp>
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
    int mRetries = 2;
    int mSockDelay = 5;
    std::chrono::milliseconds mTimeout {100};
    Traceroute::ProbeSender * mProbeSender;
    Traceroute::Icmp::IcmpDataSender * mDataSender;
    Traceroute::Icmp::IcmpResponseValidator * mResponseValidator;
    void SetUp() override
    {
        mDestinationText = "8.8.8.8";
        mDestinationAddr = Traceroute::SocketAddress{mDestinationText};
        mSource = Traceroute::SocketAddress("192.168.132.129");
        mFamily = mDestinationAddr.getFamily();       
        mDataSender = new Traceroute::Icmp::IcmpDataSender(mFamily, mSource, mSockDelay);
        mResponseValidator = new Traceroute::Icmp::IcmpResponseValidator;
        mProbeSender = new Traceroute::ProbeSender(mDataSender,mResponseValidator);
    }
    void TearDown() override
    {
        delete mDataSender;
        delete mResponseValidator;
        delete mProbeSender;
    }
    
};

TEST_F(LiveIcmpTest_8888, GotResponse)
{
    std::vector<Traceroute::ProbeResultContainer> probes;
    for (int ttl = 1; ttl < 32; ++ttl)
    {
        auto packet = Traceroute::PacketBuilder::CreateIcmpPacket(mSource, mDestinationAddr);
        auto result = mProbeSender->beginProbing(&packet, ttl, mRetries, mTimeout);
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
