
#include "Traceroute/DataSender.hpp"
#include "Traceroute/GreedyTraceroute.hpp"
#include "Traceroute/Packet/IcmpPacket.hpp"
#include "Traceroute/PacketFactory/IcmpPacketFactory.hpp"
#include "Traceroute/Probe.hpp"
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
namespace greedy {

using namespace testing;
using namespace chrono_literals;
using namespace packet;

static auto systemClockFake = std::make_shared<SystemClockFake>();

ACTION_P2(waitAndReturn, waitDuration, valueToBeReturned)
{
    systemClockFake->advanceBy(waitDuration);
    return valueToBeReturned;
}

class GreedyTest : public Test
{
  public:
    NiceMock<PacketFactoryMock> packetFactoryMock;
    NiceMock<DataSenderMock>    dataSenderMock;
    NiceMock<PacketMock>        packetMock;

    const SocketAddress source{"192.168.0.1"};
    const SocketAddress destination{"1.1.1.1"};
    const size_t        ArbitraryResponseSize = 1234;
    const int           ArbitraryProtocol     = 2345;

    GreedyTraceroute underTest;

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

    GreedyTest() : underTest(packetFactoryMock, dataSenderMock, systemClockFake)
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

TEST_F(GreedyTest, onTimeout_waitedFor_isEqualTo_timeout)
{
    EXPECT_CALL(dataSenderMock, tryReceiving(_, _, _)).WillOnce(waitAndReturn(timeout, nullopt));

    const auto probes = BeginProbing();

    ASSERT_THAT(probes, SizeIs(1));
    ASSERT_THAT(probes.front().probeResults(), SizeIs(1));
    const auto &probeResult = probes.front().probeResults().front();
    ASSERT_THAT(probeResult.success(), Eq(false));
    EXPECT_THAT(probeResult.waitedFor(), Eq(timeout));
}

} // namespace greedy
} // namespace traceroute