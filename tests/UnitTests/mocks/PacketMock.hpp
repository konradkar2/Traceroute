#pragma once

#include <Traceroute/interface/Packet.hpp>
#include <gmock/gmock.h>

namespace traceroute
{
class PacketMock : public Packet
{
  public:
   // MOCK_METHOD0(std::string, serialize, (), (const, override));
};
} // namespace traceroute
