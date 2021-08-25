#pragma once
#include "../HeaderTypes.hpp"
#include "../SocketAddress.hpp"
#include "../Packet.hpp"

namespace Traceroute
{

    class TcpPacket : public Packet
    {
    public:
        TcpPacket(const SocketAddress &source,
                  const SocketAddress &destination, int dport);
        const TcpHeader &getTcpHeader() const;
        std::string serialize() const override;
        ~TcpPacket() {}

    private:
        TcpHeader mTcpHeader;
    };
}