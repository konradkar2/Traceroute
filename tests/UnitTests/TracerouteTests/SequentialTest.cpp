
#include "Traceroute/DataSender.hpp"
#include "Traceroute/Packet/IcmpPacket.hpp"
#include "Traceroute/PacketFactory/IcmpPacketFactory.hpp"
#include "Traceroute/Probe.hpp"
#include "Traceroute/SequentialTraceroute.hpp"
#include "Traceroute/SocketAddress.hpp"
#include "mocks/DataSenderMock.hpp"
#include "mocks/PacketFactoryMock.hpp"
#include "mocks/PacketMock.hpp"
#include "mocks/ResponseValidator.hpp"
#include "mocks/SystemClockFake.hpp"
#include <chrono>
#include <compare>
#include <gmock/gmock.h>
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

class SequentialTest : public Test
{
  public:
    NiceMock<PacketFactoryMock> packetFactoryMock;
    NiceMock<DataSenderMock>    dataSenderMock;
    NiceMock<PacketMock>        packetMock;

    const SocketAddress source{"192.168.0.1"};
    const SocketAddress destination{"1.1.1.1"};
    const size_t        ArbitraryResponseSize = 1234;
    const int           ArbitraryProtocol     = 2345;

    SequentialTraceroute underTest;

    // default params - by default send 1 packet (which is one retry)
    // with ttl set to 1
    int                             ttlStart = 1;
    int                             ttlStop  = 1;
    int                             retries  = 1;
    const std::chrono::microseconds timeout  = 100ms;

    std::vector<TracerouteResult> BeginProbing()
    {
        return underTest.beginProbing(ttlStart, ttlStop, retries, timeout);
    }

    SequentialTest() : underTest(packetFactoryMock, dataSenderMock, systemClockFake)
    {
        ON_CALL(packetMock, isValid(_, _)).WillByDefault(Return(false));
        ON_CALL(packetFactoryMock, createPacketProxy()).WillByDefault(Invoke([this]() {
            return createPacketMockProxy();
        }));
        ON_CALL(dataSenderMock, tryReceiving(_, _, _)).WillByDefault(waitAndReturn(timeout, nullopt));
        ON_CALL(dataSenderMock, trySending(_, _, _)).WillByDefault(Return(true));
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
    InSequence s;
};

TEST_F(SequentialTest, onTimeout_waitedFor_isEqualTo_timeout)
{
    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _)).WillOnce(waitAndReturn(timeout, nullopt));

    const auto probes = BeginProbing();

    ASSERT_THAT(probes, SizeIs(1));
    ASSERT_THAT(probes.front().probeResults(), SizeIs(1));
    const auto &probeResult = probes.front().probeResults().front();
    ASSERT_THAT(probeResult.success(), Eq(false));
    EXPECT_THAT(probeResult.waitedFor(), Eq(timeout));
}

TEST_F(SequentialTest, ttlRangeResultsInProbeCount)
{
    ttlStart                     = 2;
    ttlStop                      = 5;
    const int expectedProbeCount = ttlStop - ttlStart + 1;

    const auto probes = BeginProbing();

    EXPECT_THAT(probes.size(), Eq(expectedProbeCount));
}

TEST_F(SequentialTest, forEachRetryTtlIsNotChangedButPacketfactoryIsInvoked)
{
    ttlStart = 1;
    ttlStop  = 1;
    retries  = 3;

    EXPECT_CALL(packetFactoryMock, createPacketProxy()).Times(1);
    EXPECT_CALL(dataSenderMock, trySending(_, 1, _)).Times(1);
    EXPECT_CALL(packetFactoryMock, createPacketProxy()).Times(1);
    EXPECT_CALL(dataSenderMock, trySending(_, 1, _)).Times(1);
    EXPECT_CALL(packetFactoryMock, createPacketProxy()).Times(1);
    EXPECT_CALL(dataSenderMock, trySending(_, 1, _)).Times(1);

    auto probes = BeginProbing();
}

TEST_F(SequentialTest, responseAddressNotAssignedOnTimeout)
{
    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _)).WillOnce(waitAndReturn(timeout, nullopt));

    const auto probes = BeginProbing();

    ASSERT_THAT(probes, SizeIs(1));
    ASSERT_THAT(probes.front().probeResults(), SizeIs(1));
    const auto &probeResult = probes.front().probeResults().front();
    ASSERT_THAT(probeResult.success(), Eq(false));
    EXPECT_THAT(probeResult.responseSender().has_value(), Eq(false));
}

TEST_F(SequentialTest, responseAddressAssigned)
{
    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{destination}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, isValid(_, _)).WillOnce(Return(true));

    const auto probes = BeginProbing();

    ASSERT_THAT(probes, SizeIs(1));
    ASSERT_THAT(probes.front().probeResults(), SizeIs(1));
    const auto &probeResult = probes.front().probeResults().front();
    EXPECT_THAT(probeResult.success(), Eq(true));
    ASSERT_TRUE(probeResult.responseSender().has_value());
    EXPECT_THAT(probeResult.responseSender(), Eq(destination));
}

TEST_F(SequentialTest, onTimeLeftSmallerThan_MinTimeWaitForResponse_receiveFromNotInvoked)
{
    // 5us are left for polling, which is below MinTimeWaitForResponse
    auto timeToPass = timeout - 5us;

    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _)).WillOnce(waitAndReturn(timeToPass, nullopt));
    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _)).Times(0);

    BeginProbing();
}

TEST_F(SequentialTest, responseIsAssignedToResultWithValidWaitedFor)
{
    auto returnAfter = 50ms;
    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _))
        .WillOnce(waitAndReturn(returnAfter,
                                ResponseInfo{SocketAddress{destination}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, isValid(_, _)).WillOnce(Return(true));

    const auto probes = BeginProbing();

    ASSERT_THAT(probes, SizeIs(1));
    ASSERT_THAT(probes.front().probeResults(), SizeIs(1));
    const auto &probeResult = probes.front().probeResults().front();
    ASSERT_TRUE(probeResult.responseSender().has_value());
    EXPECT_THAT(probeResult.responseSender(), Eq(destination));
    EXPECT_THAT(probeResult.waitedFor(), Eq(returnAfter));
}

TEST_F(SequentialTest, waitedForIsSumOfPreviousInvalidResponses)
{
    auto t1 = 10ms, t2 = 30ms, t3 = 40ms;

    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _)).WillOnce(waitAndReturn(t1, nullopt));
    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _)).WillOnce(waitAndReturn(t2, nullopt));
    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _))
        .WillOnce(
            waitAndReturn(t3, ResponseInfo{SocketAddress{destination}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, isValid(_, _)).WillOnce(Return(true));

    const auto probes = BeginProbing();

    ASSERT_THAT(probes, SizeIs(1));
    ASSERT_THAT(probes.front().probeResults(), SizeIs(1));
    const auto &probeResult = probes.front().probeResults().back();
    EXPECT_THAT(probeResult.success(), Eq(true));
    EXPECT_THAT(probeResult.waitedFor(), Eq(t1 + t2 + t3));
}

TEST_F(SequentialTest, timeoutBetweenTwoSuccessfulProbes)
{
    ttlStop = 3;
    // r2 is timeouted
    const SocketAddress r1{"1.2.3.4"};
    const SocketAddress r3{"2.3.4.5"};

    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{r1}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, isValid(_, _)).WillOnce(Return(true));

    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _)).WillOnce(waitAndReturn(timeout, nullopt));
    EXPECT_CALL(packetMock, isValid(_, _)).Times(0);

    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _))
        .WillOnce(Return(ResponseInfo{SocketAddress{r3}, ArbitraryProtocol, ArbitraryResponseSize}));
    EXPECT_CALL(packetMock, isValid(_, _)).WillOnce(Return(true));

    auto probes = BeginProbing();

    ASSERT_THAT(probes, SizeIs(3));
    auto probe = probes.begin();

    ASSERT_THAT(probe->probeResults(), SizeIs(1));
    auto probeResult = probe->probeResults().begin();
    EXPECT_THAT(probeResult->success(), Eq(true));
    EXPECT_THAT(probeResult->responseSender(), Eq(r1));

    std::advance(probe, 1);
    ASSERT_THAT(probe->probeResults(), SizeIs(1));
    probeResult = probe->probeResults().begin();
    EXPECT_THAT(probeResult->success(), Eq(false));
    EXPECT_THAT(probeResult->responseSender().has_value(), Eq(false));

    std::advance(probe, 1);
    ASSERT_THAT(probe->probeResults(), SizeIs(1));
    probeResult = probe->probeResults().begin();
    EXPECT_THAT(probeResult->success(), Eq(true));
    EXPECT_THAT(probeResult->responseSender().has_value(), Eq(true));
    EXPECT_THAT(probeResult->responseSender(), Eq(r3));
}

} // namespace traceroute