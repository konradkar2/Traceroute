#include <Traceroute/IValidateResponse.hpp>

namespace Traceroute
{
    namespace Icmp
    {
        namespace V6
        {

            class Icmp6ResponseValidator : public IValidateResponse
            {
            public:                
                static IValidateResponse & instance()
                {
                    static Icmp6ResponseValidator mValidator;
                    return mValidator;
                }
                bool isResponseValid(const Packet &request, const SocketAddress &client,
                                     int protocol, const char *response, size_t responseSize) override;
            private:
                Icmp6ResponseValidator() = default;
            };
        }
    }
}