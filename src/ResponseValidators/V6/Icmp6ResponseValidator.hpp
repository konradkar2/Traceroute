#include <Traceroute/IValidateResponse.hpp>

namespace traceroute::responseValidators::v6
{

    class Icmp6ResponseValidator : public IValidateResponse
    {
    public:
        bool validate(const Packet &request, const SocketAddress &client,
                             int protocol, const char *response, size_t responseSize) override;
    };
}