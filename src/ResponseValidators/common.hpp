#include <Traceroute/HeaderTypes.hpp>
#include <cstdint>


namespace traceroute
{
bool isEchoReply(const char * icmpHeader);
bool isTimeExceeded(const char * icmpHeader);
bool validateSession(const char *icmpReply, const IcmpHeader & icmpEcho);
int getIcmpType(const char * icmpHeader);
} // namespace traceroute