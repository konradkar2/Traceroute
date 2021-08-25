#include "Utils.hpp"
#include <string>
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <netinet/ip_icmp.h>

namespace ResponseValidatorsTests
{
    void fillIPv4Header(const Traceroute::SocketAddress &source, const Traceroute::SocketAddress &destination, int ihl,
                        int protocol, Traceroute::Ipv4Header *ipHeader)
    {
        ipHeader->version = 4;
        ipHeader->ihl = ihl;
        ipHeader->protocol = protocol;       
        ipHeader->saddr = reinterpret_cast<const sockaddr_in *>(source.sockaddrP())->sin_addr.s_addr;
        ipHeader->daddr = reinterpret_cast<const sockaddr_in *>(destination.sockaddrP())->sin_addr.s_addr;
    }
}
