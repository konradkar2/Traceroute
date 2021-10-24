#include <Traceroute/HeaderTypes.hpp>
#include <cstdint>


namespace traceroute
{
bool validateSession(const char *icmpReply, const IcmpHeader & icmpEcho);
bool validateSameSequence(const char * tcpReply, const TcpHeader & tcpEcho);
int getIcmpType(const char * icmpHeader);
size_t getIpHeaderSize(const char *ptr);
} // namespace traceroute