#pragma once

#include "mocks/PacketMock.hpp"
#include <Traceroute/interface/IPacketFactory.hpp>
#include <gmock/gmock-nice-strict.h>
#include <gmock/gmock.h>

using testing::NiceMock;

namespace traceroute {
class PacketFactoryMock : public IPacketFactory
{
  public:
    std::unique_ptr<Packet> createPacket() override
    {
        return std::unique_ptr<PacketMockProxy>(createPacketProxy());
    }
    MOCK_METHOD0(createPacketProxy, PacketMockProxy *());
};
} // namespace traceroute