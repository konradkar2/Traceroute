#include "ChecksumCalculator.hpp"
#include "PodSerializer.hpp"
#include <Traceroute/Packet.hpp>
#include <stdexcept>
namespace traceroute
{

Packet::Packet(const SocketAddress &source, const SocketAddress &destination)
    : mSourceAddress(source), mDestinationAddress(destination)
{
    if (destination.family() != source.family())
    {
        throw runtime_error("source address family doesnt match destination");
    }
}
const SocketAddress &Packet::getSourceAddress() const
{
    return mSourceAddress;
}
const SocketAddress &Packet::getDestinationAddress() const
{
    return mDestinationAddress;
}
int Packet::getFamily() const
{
    return mSourceAddress.family();
}

} // namespace traceroute
