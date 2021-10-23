#include <Traceroute/ResponseValidators/ResponseValidator.hpp>

namespace traceroute::responseValidators::v6
{

class Icmp6ResponseValidator : public ResponseValidator
{
  public:
    bool validateFields(const Packet &request, const SocketAddress &client, const char *response,
                  size_t responseSize) override;
    bool validateProtocol(int protocol) override;
    bool validateSize(size_t size) override;
};
} // namespace traceroute::responseValidators::v6