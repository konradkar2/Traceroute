#include <Traceroute/IValidateResponse.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <memory>
#include <cstdint>
namespace traceroute
{
    namespace responseValidators
    {
        class IcmpResponseValidator : public IValidateResponse
        {
        public:
            IcmpResponseValidator();
            bool validate(const Packet &request, const SocketAddress &client,
                                 int protocol, const char *response, size_t responseSize) override;
        private:
            std::unique_ptr<IValidateResponse> mIcmp4ResponseValidator;
            std::unique_ptr<IValidateResponse> mIcmp6ResponseValidator;
        };
    }
}