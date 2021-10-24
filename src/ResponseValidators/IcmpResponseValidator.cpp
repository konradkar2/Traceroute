#include "IcmpToIcmpResponseValidator.hpp"
#include "common.hpp"
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
namespace traceroute::responseValidators
{

IcmpResponseValidator::IcmpResponseValidator()
{
    mIcmpResponseValidator = std::make_unique<IcmpToIcmpResponseValidator>();
}
bool IcmpResponseValidator::validate(const Packet &request, const SocketAddress &client, int protocol,
                                     const char *response, size_t responseSize)
{
    if (client.isV4())
    {
        auto ipHeaderSize = getIpHeaderSize(response);
        response += ipHeaderSize;
        responseSize -= ipHeaderSize;
    }
    return mIcmpResponseValidator->validate(request, client, protocol, response, responseSize);
}

} // namespace traceroute::responseValidators
