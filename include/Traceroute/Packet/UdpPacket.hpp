#pragma once
#include "../HeaderTypes.hpp"
#include "../SocketAddress.hpp"
#include "../Packet.hpp"

namespace traceroute::packet
{
    class UdpPacket : public Packet
    {
    public:
        UdpPacket(const SocketAddress &source,
                  const SocketAddress &destination, int dport);
        const UdpHeader &GetUdpHeader() const;
        std::string serialize() const override;
        ~UdpPacket() {}

    private:
        UdpHeader mUdpHeader;
    };

}