#include <Traceroute/HeaderTypes.hpp>
#include <cassert>
namespace traceroute::responseValidators::v4
{
size_t getIpHeaderSize(const char *ipHdr)
{
    const Ipv4Header *ip4header = reinterpret_cast<const Ipv4Header *>(ipHdr);
    assert(ip4header->version == 4);
    auto iphdr_size = ip4header->ihl << 2;
    return iphdr_size;
}
} // namespace traceroute::responseValidators::v4
