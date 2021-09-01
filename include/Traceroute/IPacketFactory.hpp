#pragma once

#include <memory>
#include "Packet.hpp"

namespace traceroute
{
    class IPacketFactory
    {
    public:
        virtual std::unique_ptr<Packet> createPacket() = 0;
        virtual ~IPacketFactory() = default;
    };

}