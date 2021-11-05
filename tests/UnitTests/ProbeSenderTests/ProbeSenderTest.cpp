
#include "Traceroute/ProbeSender.hpp"
#include "Traceroute/DataSender.hpp"
#include "Traceroute/Packet/IcmpPacket.hpp"
#include "Traceroute/PacketFactory/IcmpPacketFactory.hpp"
#include "Traceroute/SocketAddress.hpp"
#include "mocks/DataSenderMock.hpp"
#include "mocks/PacketFactoryMock.hpp"
#include "mocks/ResponseValidator.hpp"
#include <Traceroute/Packet.hpp>
#include <chrono>
#include <compare>
#include <gmock/gmock-actions.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

namespace traceroute
{

using namespace testing;
using namespace chrono_literals;
using namespace packet;

ACTION(createArbitraryPacket)
{
    const SocketAddress source{"1.2.3.4"};
    const SocketAddress destination{"2.3.4.5"};
    return std::make_unique<IcmpPacket>(IcmpPacket::CreateIcmp4Packet(source, destination));
}

ACTION_P2(sleepAndReturn, sleepFor, valueToBeReturned)
{
    std::this_thread::sleep_for(sleepFor);
    return valueToBeReturned;
}

class ProbeSenderTest : public Test
{
  public:
    NiceMock<PacketFactoryMock> packetFactoryMock;
    NiceMock<DataSenderMock> dataSenderMock;
    NiceMock<ResponseValidatorMock> responseValidatorMock;

    const SocketAddress source{"192.168.0.1"};
    const SocketAddress destination{"1.1.1.1"};
    const ssize_t ArbitraryResponseSize = 1234;
    const int ArbitraryProtocol = 2345;

    ProbeSender underTest;

    // default params - by default send 1 packet (which is one retry)
    // with ttl set to 1
    int ttlStart = 1;
    int ttlStop = 1;
    int retries = 1;

    // If this is set to true receiveFrom won't be invoked,
    /// because of MinTimeWaitForResponse in ProbeSender
    int shouldTimeOutOnStart = true;
    std::chrono::microseconds timeout = 9us;

    std::vector<ProbeResultContainer> BeginProbing()
    {
        if (timeout == 9us)
        {
            shouldTimeOutOnStart == false ? timeout = 11us : 9us;
        }

        return underTest.beginProbing(ttlStart, ttlStop, retries, timeout);
    }

    ProbeSenderTest() : underTest(packetFactoryMock, dataSenderMock, responseValidatorMock)
    {
        EXPECT_CALL(packetFactoryMock, createPacket()).WillRepeatedly(createArbitraryPacket());
    }

    void SetUp() override
    {
    }
};

TEST_F(ProbeSenderTest, setsTtlOnSender)
{
    ttlStop = 2;

    InSequence s;
    EXPECT_CALL(dataSenderMock, setTtlOnSendingSocket(1)).Times(1);
    EXPECT_CALL(dataSenderMock, setTtlOnSendingSocket(2)).Times(1);

    BeginProbing();
}

TEST_F(ProbeSenderTest, onTimeout_waitedFor_IsEqualTo_timeout)
{
    timeout = 11us;

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(sleepAndReturn(timeout,
                                 ResponseInfo{SocketAddress{destination}, ArbitraryProtocol, ArbitraryResponseSize}));

    auto probes = BeginProbing();

    EXPECT_THAT(probes.front().getResults().front().waitedFor, Eq(timeout));
}

TEST_F(ProbeSenderTest, responseAddressNotAssignedOnTimeout)
{
    timeout = 11us;

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).WillOnce(sleepAndReturn(timeout, nullopt));
    auto probes = BeginProbing();

    ASSERT_THAT(probes.size(), Eq(1));
    EXPECT_THAT(probes.front().GetResponseAddr().has_value(), Eq(false));
}

TEST_F(ProbeSenderTest, properResponseIsAssignedToResult)
{
    shouldTimeOutOnStart = false;

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{destination}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(responseValidatorMock, validate).WillOnce(Return(true));

    auto probes = BeginProbing();

    ASSERT_THAT(probes.size(), Eq(1));
    ASSERT_THAT(probes.front().GetResponseAddr().has_value(), Eq(true));
    EXPECT_THAT(probes.front().GetResponseAddr(), Eq(destination));
}

TEST_F(ProbeSenderTest, timeoutBetweenTwoSuccessfulProbes)
{
    ttlStop = 3;
    // r2 is timeouted
    const SocketAddress r1{"1.2.3.4"};
    const SocketAddress r3{"2.3.4.5"};
    shouldTimeOutOnStart = false;

    InSequence s;
    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{r1}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(responseValidatorMock, validate).WillOnce(Return(true));

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).WillOnce(sleepAndReturn(15us, nullopt));

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{r3}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(responseValidatorMock, validate).WillOnce(Return(true));

    auto probes = BeginProbing();

    ASSERT_THAT(probes.size(), Eq(3));
    auto probe = probes.begin();
    EXPECT_THAT(probe->GetResponseAddr(), Eq(r1));
    std::advance(probe, 1);
    EXPECT_THAT(probe->GetResponseAddr().has_value(), Eq(false));
    std::advance(probe, 1);
    EXPECT_THAT(probe->GetResponseAddr(), Eq(r3));
}

} // namespace traceroute