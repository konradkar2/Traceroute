#include "Traceroute/Packet/TcpPacket.hpp"
#include "Traceroute/ResponseValidators/ResponseValidator.hpp"
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/interface/IValidateResponse.hpp>
#include <cstdint>
#include <memory>
namespace traceroute {
namespace responseValidators {
class TcpResponseValidator : public IValidateResponse
{
  public:
    TcpResponseValidator(const packet::TcpPacket &tcpPacket);
    bool validate(const ResponseInfo &responseInfo, const char *response) override;

  private:
    const packet::TcpPacket &mTcpPacket;
};
} // namespace responseValidators
} // namespace traceroute