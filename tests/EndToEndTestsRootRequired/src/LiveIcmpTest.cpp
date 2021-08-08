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
    void SetUp() override
    {
        mDestinationText = "8.8.8.8";
        mDestinationAddr = Traceroute::SocketAddress{mDestinationText};
        mSource = Traceroute::SocketAddress("192.168.132.129");
        mFamily = mDestinationAddr.getFamily();       
        mProbeSender = new Traceroute::ProbeSender(std::make_unique<Traceroute::Icmp::IcmpDataSender>(mFamily,mSource,mRetries),
					std::make_unique<Traceroute::Icmp::IcmpResponseValidator>());
    }
    void TearDown() override
    {
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
