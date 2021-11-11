#include <Traceroute/interface/IValidateResponse.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <cstdint>
#include <memory>
namespace traceroute
{
namespace responseValidators
{
class TcpResponseValidator : public IValidateResponse
{
  public:
    TcpResponseValidator();
    bool validate(const Packet &request, const SocketAddress &client, int protocol, const char *response,
                  size_t responseSize) override;

  private:
    std::unique_ptr<IValidateResponse> mTcpToTcpValidator;
    std::unique_ptr<IValidateResponse> mIcmpToTcpValidator;
};
} // namespace responseValidators
} // namespace traceroute