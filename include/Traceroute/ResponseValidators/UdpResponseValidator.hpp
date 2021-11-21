#include "Traceroute/Packet/UdpPacket.hpp"
#include "Traceroute/ResponseValidators/ResponseValidator.hpp"
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/interface/IValidateResponse.hpp>
#include <cstdint>
namespace traceroute {
namespace responseValidators {
class UdpResponseValidator : public ResponseValidator
{
  public:
    UdpResponseValidator(const packet::UdpPacket & udpPacket);

  protected:
    bool validateFields(const ResponseInfo &responseInfo, const char *response) override;
    bool validateProtocol(int protocol) override;
    bool validateSize(size_t size) override;

  private:
    const packet::UdpPacket &mUdpPacket;
};
} // namespace responseValidators
} // namespace traceroute