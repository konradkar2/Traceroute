#include <Traceroute/IValidateResponse.hpp>

namespace Traceroute
{
    namespace ResponseValidators
    {
        namespace V4
        {
            class Icmp4ResponseValidator : public IValidateResponse
            {
            public:
                bool isResponseValid(const Packet &request, const SocketAddress &client,
                                     int protocol, const char *response, size_t responseSize) override;
            };
        }
    }
}