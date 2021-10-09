#pragma once
#include <Traceroute/IValidateResponse.hpp>

namespace traceroute::responseValidators::v4
{
    class Icmp4ResponseValidator : public IValidateResponse
    {
    public:
        bool isResponseValid(const Packet &request, const SocketAddress &client,
                             int protocol, const char *response, size_t responseSize) override;
    };
}
