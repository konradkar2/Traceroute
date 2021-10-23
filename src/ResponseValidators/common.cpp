#include "common.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <cassert>
#include <cstdint>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>

namespace traceroute
{
namespace
{
const IcmpHeader *toIcmpHeaderT(const char *p)
{
    return reinterpret_cast<const IcmpHeader *>(p);
}
} // namespace


bool validateSession(const char *icmpReply, const IcmpHeader & icmpEcho)
{
    return toIcmpHeaderT(icmpReply)->id == icmpEcho.id;
}

int getIcmpType(const char *icmpHeader)
{
    return toIcmpHeaderT(icmpHeader)->type;
}
} // namespace traceroute