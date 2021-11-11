#include <Traceroute/interface/IValidateResponse.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <cstdint>
#include <memory>

namespace traceroute
{
namespace responseValidators
{
class IcmpResponseValidator : public IValidateResponse
{
  public:
    IcmpResponseValidator();
    bool validate(const Packet &request, const SocketAddress &client, int protocol, const char *response,
                  size_t responseSize) override;
  private:
    std::unique_ptr<IValidateResponse> mIcmpResponseValidator;
};
} // namespace responseValidators
} // namespace traceroute