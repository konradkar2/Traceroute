#include "utils/ChecksumCalculator.hpp"
#include "utils/Serialize.hpp"
#include <Traceroute/Packet/TcpPacket.hpp>
#include <Traceroute/ResponseValidators/TcpResponseValidator.hpp>
#include <arpa/inet.h>
#include <stdexcept>

namespace traceroute::packet {

std::string TcpPacket::serialize() const
{
    return utils::serialize<TcpHeader>(mTcpHeader);
}

const TcpHeader &TcpPacket::getTcpHeader() const
{
    return mTcpHeader;
}

TcpPacket::TcpPacket(const SocketAddress &source, const SocketAddress &destination, int dport)
    : Packet(source, destination), mResponseValidator{std::make_unique<responseValidators::TcpResponseValidator>(*this)}
{
    mTcpHeader.source   = htons(50000 + rand() % 15001);
    mTcpHeader.dest     = htons(dport);
    mTcpHeader.seq      = htonl(rand() % 65001); // generate random sequence number
    mTcpHeader.ack_seq  = 0;                     // ack sequence should be 0
    mTcpHeader.window   = htons(5760);           // set window size
    mTcpHeader.th_x2    = 0;
    mTcpHeader.th_off   = 5;
    mTcpHeader.th_flags = 0x02;
    mTcpHeader.check    = 0;
    if (source.isV4())
    {
        mTcpHeader.check = utils::ChecksumCalculator::computeTCPHeaderChecksum(mTcpHeader, source, destination);
    }
}

bool TcpPacket::validate(const ResponseInfo &responseInfo, const char *response)
{
    return mResponseValidator->validate(responseInfo, response);
}

} // namespace traceroute::packet