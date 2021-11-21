
#include "Traceroute/ProbeSender.hpp"
#include "Traceroute/DataSender.hpp"
#include "Traceroute/Packet/IcmpPacket.hpp"
#include "Traceroute/PacketFactory/IcmpPacketFactory.hpp"
#include "Traceroute/SocketAddress.hpp"
#include "mocks/DataSenderMock.hpp"
#include "mocks/PacketFactoryMock.hpp"
#include "mocks/PacketMock.hpp"
#include "mocks/ResponseValidator.hpp"
#include "mocks/SystemClockFake.hpp"
#include <chrono>
#include <compare>
#include <gmock/gmock-actions.h>
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-spec-builders.h>
#include <gmock/gmock.h>
#include <gtest/gtest-matchers.h>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

namespace traceroute {

using namespace testing;
using namespace chrono_literals;
using namespace packet;

static auto systemClockFake = std::make_shared<SystemClockFake>();

ACTION_P2(waitAndReturn, waitDuration, valueToBeReturned)
{
    systemClockFake->advanceBy(waitDuration);
    return valueToBeReturned;
}

class ProbeSenderTest : public Test
{
  public:
    NiceMock<PacketFactoryMock> packetFactoryMock;
    NiceMock<DataSenderMock>    dataSenderMock;
    NiceMock<PacketMock>        packetMock;

    const SocketAddress source{"192.168.0.1"};
    const SocketAddress destination{"1.1.1.1"};
    const size_t        ArbitraryResponseSize = 1234;
    const int           ArbitraryProtocol     = 2345;

    ProbeSender underTest;

    // default params - by default send 1 packet (which is one retry)
    // with ttl set to 1
    int                             ttlStart = 1;
    int                             ttlStop  = 1;
    int                             retries  = 1;
    const std::chrono::microseconds timeout  = 100ms;

    const std::chrono::microseconds thisWillNotTimeout = 99ms;

    std::vector<ProbeResultContainer> BeginProbing()
    {
        return underTest.beginProbing(ttlStart, ttlStop, retries, timeout);
    }

    ProbeSenderTest() : underTest(packetFactoryMock, dataSenderMock, systemClockFake)
    {
        ON_CALL(packetMock, validate(_, _)).WillByDefault(Return(false));
        ON_CALL(packetFactoryMock, createPacketProxy()).WillByDefault(Invoke([this]() {
            return createPacketMockProxy();
        }));
        ON_CALL(dataSenderMock, receiveFrom(_, _, _)).WillByDefault(waitAndReturn(timeout, nullopt));
    }
    PacketMockProxy *createPacketMockProxy()
    {
        PacketMockProxy *proxy = new PacketMockProxy();
        proxy->setMock(&packetMock);
        return proxy;
    }

    void SetUp() override
    {
    }
    // InSequence s;
};

TEST_F(ProbeSenderTest, setsTtlOnSender)
{
    ttlStop = 2;

    EXPECT_CALL(dataSenderMock, setTtlOnSendingSocket(1)).Times(1);
    EXPECT_CALL(dataSenderMock, setTtlOnSendingSocket(2)).Times(1);

    BeginProbing();
}

TEST_F(ProbeSenderTest, onTimeout_waitedFor_isEqualTo_timeout)
{
    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).WillOnce(waitAndReturn(timeout, nullopt));

    auto probes = BeginProbing();

    ASSERT_THAT(probes.front().getResults().front().success, Eq(false));
    EXPECT_THAT(probes.front().getResults().front().waitedFor, Eq(timeout));
}

TEST_F(ProbeSenderTest, ttlRangeResultsInProbeCount)
{
    ttlStart                     = 2;
    ttlStop                      = 5;
    const int expectedProbeCount = ttlStop - ttlStart + 1;

    auto probes = BeginProbing();

    EXPECT_THAT(probes.size(), Eq(expectedProbeCount));
}

TEST_F(ProbeSenderTest, forEachRetryTtlIsNotChangedButpacketfactoryIsInvoked)
{
    ttlStart = 1;
    ttlStop  = 1;
    retries  = 3;

    EXPECT_CALL(dataSenderMock, setTtlOnSendingSocket(1)).Times(1);
    EXPECT_CALL(packetFactoryMock, createPacketProxy()).Times(3);

    auto probes = BeginProbing();
}

TEST_F(ProbeSenderTest, responseAddressNotAssignedOnTimeout)
{
    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).WillOnce(waitAndReturn(timeout, nullopt));

    auto probes = BeginProbing();

    ASSERT_THAT(probes.front().getResults().front().success, Eq(false));
    EXPECT_THAT(probes.front().GetResponseAddr().has_value(), Eq(false));
}

TEST_F(ProbeSenderTest, responseAddressAssigned)
{
    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{destination}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, validate(_, _)).WillOnce(Return(true));

    auto probe = BeginProbing().front();

    ASSERT_THAT(probe.getResults().front().success, Eq(true));
    ASSERT_THAT(probe.GetResponseAddr().has_value(), Eq(true));
    EXPECT_THAT(probe.GetResponseAddr(), Eq(destination));
}

TEST_F(ProbeSenderTest, onTimeLeftSmallerThan_MinTimeWaitForResponse_receiveFromNotInvoked)
{
    // 5us are left for polling, which is below MinTimeWaitForResponse
    auto timeToPass = timeout - 5us;

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).WillOnce(waitAndReturn(timeToPass, nullopt));
    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).Times(0);

    BeginProbing();
}

TEST_F(ProbeSenderTest, responseIsAssignedToResultWithValidWaitedFor)
{
    auto returnAfter = 50ms;
    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(waitAndReturn(returnAfter,
                                ResponseInfo{SocketAddress{destination}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, validate(_, _)).WillOnce(Return(true));

    auto probe = BeginProbing().front();

    EXPECT_THAT(probe.GetResponseAddr(), Eq(destination));
    EXPECT_THAT(probe.getResults().front().waitedFor, Eq(returnAfter));
}

TEST_F(ProbeSenderTest, waitedForIsSumOfPreviousInvalidResponses)
{
    auto t1 = 10ms, t2 = 30ms, t3 = 40ms;

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).WillOnce(waitAndReturn(t1, nullopt));
    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).WillOnce(waitAndReturn(t2, nullopt));
    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(
            waitAndReturn(t3, ResponseInfo{SocketAddress{destination}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, validate(_, _)).WillOnce(Return(true));

    auto probe = BeginProbing().back();

    ASSERT_THAT(probe.getResults().front().success, Eq(true));
    EXPECT_THAT(probe.getResults().front().waitedFor, Eq(t1 + t2 + t3));
}

TEST_F(ProbeSenderTest, timeoutBetweenTwoSuccessfulProbes)
{
    ttlStop = 3;
    // r2 is timeouted
    const SocketAddress r1{"1.2.3.4"};
    const SocketAddress r3{"2.3.4.5"};

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{r1}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, validate(_, _)).WillOnce(Return(true));

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _)).WillOnce(waitAndReturn(timeout, nullopt));
    EXPECT_CALL(packetMock, validate(_, _)).WillOnce(Return(false));

    EXPECT_CALL(dataSenderMock, receiveFrom(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{r3}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, validate(_, _)).WillOnce(Return(true));
    auto probes = BeginProbing();

    auto probe = probes.begin();
    EXPECT_THAT(probe->GetResponseAddr(), Eq(r1));
    std::advance(probe, 1);
    EXPECT_THAT(probe->getResults().front().success, Eq(false));
    EXPECT_THAT(probe->GetResponseAddr().has_value(), Eq(false));
    std::advance(probe, 1);
    EXPECT_THAT(probe->GetResponseAddr(), Eq(r3));
}

} // namespace traceroute