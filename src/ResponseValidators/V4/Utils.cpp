#include <Traceroute/HeaderTypes.hpp>
#include <cassert>
namespace traceroute::responseValidators::v4
{
    const char *skipIpHeader(const char *ipHdr)
    {
        const Ipv4Header *ip4header = reinterpret_cast<const Ipv4Header *>(ipHdr);
        assert(ip4header->version == 4);
        auto iphdr_size = ip4header->ihl << 2;
        ipHdr += iphdr_size;
        return ipHdr;
    }
}
