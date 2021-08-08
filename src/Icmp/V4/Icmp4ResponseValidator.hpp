#include <Traceroute/IValidateResponse.hpp>

namespace Traceroute
{
    namespace Icmp
    {
        namespace V4
        {
            class Icmp4ResponseValidator : public IValidateResponse
            {
            public:                
                static IValidateResponse & instance()
                {
                    static Icmp4ResponseValidator mValidator;
                    return mValidator;
                }
                bool isResponseValid(const Packet &request, const SocketAddress &client,
                                     int protocol, const char *response, size_t responseSize) override;
            private:
                Icmp4ResponseValidator() = default;
            };
        }
    }
}