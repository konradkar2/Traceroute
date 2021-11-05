#pragma once

#include "Traceroute/IPacketFactory.hpp"
#include <gmock/gmock.h>

namespace traceroute
{
class PacketFactoryMock : public IPacketFactory
{
  public:
    MOCK_METHOD(std::unique_ptr<Packet>, createPacket, (), (override));
};
} // namespace traceroute