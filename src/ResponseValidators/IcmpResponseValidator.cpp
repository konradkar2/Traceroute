#include "V4/Icmp4ResponseValidator.hpp"
#include "V6/Icmp6ResponseValidator.hpp"
#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>

namespace traceroute::responseValidators
{

IcmpResponseValidator::IcmpResponseValidator()
{
    mIcmp4ResponseValidator = std::make_unique<v4::Icmp4ResponseValidator>();
    mIcmp6ResponseValidator = std::make_unique<v6::Icmp6ResponseValidator>();
}
bool IcmpResponseValidator::validate(const Packet &request, const SocketAddress &client, int protocol,
                                     const char *response, size_t responseSize)
{
    bool isResponseValid = false;
    if (responseSize == 0)
        return false;
    if (client.isV4())
        isResponseValid = mIcmp4ResponseValidator->validate(request, client, protocol, response, responseSize);
    else
        isResponseValid = mIcmp6ResponseValidator->validate(request, client, protocol, response, responseSize);

    return isResponseValid;
}

} // namespace traceroute::responseValidators
