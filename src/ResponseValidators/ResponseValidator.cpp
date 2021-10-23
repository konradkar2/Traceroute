#include <Traceroute/ResponseValidators/ResponseValidator.hpp>

namespace traceroute::responseValidators
{

bool ResponseValidator::validate(const Packet &request, const SocketAddress &client, int protocol, const char *response,
                                 size_t responseSize)
{
    return validateFields(request, client, response, responseSize) && validateProtocol(protocol) &&
           validateSize(responseSize);
}


} // namespace traceroute::responseValidators