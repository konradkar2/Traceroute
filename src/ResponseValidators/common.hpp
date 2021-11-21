#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/HeaderTypes.hpp>
#include <cstdint>

namespace traceroute {

std::pair<ResponseInfo, const char *> skipIpHeader(const ResponseInfo &responseInfo, const char *response);
bool                                  isIpHeaderPresent(const ResponseInfo &responseInfo, const char *response);
bool                                  validateSession(const char *icmpReply, const IcmpHeader &icmpEcho);
bool                                  validateSameSequence(const char *tcpReply, const TcpHeader &tcpEcho);
int                                   getIcmpType(const char *icmpHeader);
size_t                                getIpHeaderSize(const char *ptr);

} // namespace traceroute