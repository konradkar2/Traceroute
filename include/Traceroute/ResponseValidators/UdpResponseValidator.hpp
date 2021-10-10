#include <Traceroute/IValidateResponse.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <cstdint>
namespace traceroute
{
    namespace responseValidators
    {
        class UdpResponseValidator : public IValidateResponse
        {
            bool validate(const Packet &request, const SocketAddress &client,
                                 int protocol, const char *response, size_t responseSize) override;
        };
    }
}