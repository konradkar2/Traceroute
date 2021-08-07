#include <Traceroute/IValidateResponse.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <cstdint>
namespace Traceroute
{
    namespace Udp
    {
        class UdpResponseValidator : public IValidateResponse
        {
            bool isResponseValid(const Packet &request, const SocketAddress &client,
                                 int protocol, const char *response, size_t responseSize) override;
        };
    }
}