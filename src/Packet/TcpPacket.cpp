#include <stdexcept>
#include "../PodSerializer.hpp"
#include "../ChecksumCalculator.hpp"
#include <Traceroute/Packet/TcpPacket.hpp>
#include <arpa/inet.h>

namespace Traceroute
{

    std::string TcpPacket::serialize() const
    {
        return PodSerializer::serialize<TcpHeader>(mTcpHeader);
    }
    const TcpHeader &TcpPacket::getTcpHeader() const
    {
        return mTcpHeader;
    }
    TcpPacket::TcpPacket(const SocketAddress &source,
                         const SocketAddress &destination, int dport)
                         :Packet(source,destination)
    {
        mTcpHeader.source = htons(50000 + rand() % 15001);
        mTcpHeader.dest = htons(dport);
        mTcpHeader.seq = htonl(rand() % 65001); // generate random sequence number
        mTcpHeader.ack_seq = 0;                 // ack sequence should be 0
        mTcpHeader.window = htons(5760);        // set window size
        mTcpHeader.th_x2 = 0;
        mTcpHeader.th_off = 5;
        mTcpHeader.th_flags = 0x02;
        mTcpHeader.check = 0;
        if (source.isV4())
        {
            mTcpHeader.check = ChecksumCalculator::computeTCPHeaderChecksum(mTcpHeader, source, destination);
        }
    }
}