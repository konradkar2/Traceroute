#include <Traceroute/interface/IValidateResponse.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <cstdint>
namespace traceroute
{
namespace responseValidators
{
class UdpResponseValidator : public IValidateResponse
{
  public:
     bool validate(const Packet &request, const SocketAddress &client, int protocol, const char *response,
                  size_t responseSize) override;
};
} // namespace responseValidators
} // namespace traceroute