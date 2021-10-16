#pragma once
#include "../HeaderTypes.hpp"
#include "../SocketAddress.hpp"
#include "../Packet.hpp"

namespace traceroute::packet
{

    class TcpPacket : public Packet
    {
    public:
        TcpPacket(const SocketAddress &source,
                  const SocketAddress &destination, int dport);
        const TcpHeader &getTcpHeader() const;
        std::string serialize() const override;

    private:
        TcpHeader mTcpHeader;
    };
}