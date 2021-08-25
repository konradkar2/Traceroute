#pragma once
#include "../HeaderTypes.hpp"
#include "../SocketAddress.hpp"
#include "../Packet.hpp"

namespace Traceroute
{
    class IcmpPacket : public Packet
    {
    public:
        static IcmpPacket CreateIcmp4Packet(const SocketAddress &source, const SocketAddress &destination);
        static IcmpPacket CreateIcmp6Packet(const SocketAddress &source, const SocketAddress &destination);
        const IcmpHeader &GetIcmpHeader() const;
        std::string serialize() const override;
        ~IcmpPacket() = default;
    private:
        IcmpPacket(const SocketAddress &source, const SocketAddress &destination);
        IcmpHeader mIcmpHeader;
    };
}