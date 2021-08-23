#include <Traceroute/ResponseValidators/IcmpResponseValidator.hpp>
#include "V4/Icmp4ResponseValidator.hpp"
#include "V6/Icmp6ResponseValidator.hpp"

namespace Traceroute
{
    namespace ResponseValidators
    {
        IcmpResponseValidator::IcmpResponseValidator()
        {
            mIcmp4ResponseValidator = std::make_unique<V4::Icmp4ResponseValidator>();
            mIcmp6ResponseValidator = std::make_unique<V6::Icmp6ResponseValidator>();
        }
        bool IcmpResponseValidator::isResponseValid(const Packet &request, const SocketAddress &client,
                                                    int protocol, const char *response, size_t responseSize)
        {
            bool isResponseValid = false;
            if(client.family() == AF_INET)
                isResponseValid = mIcmp4ResponseValidator->isResponseValid(request,client,protocol,response,responseSize);
            else
                isResponseValid = mIcmp6ResponseValidator->isResponseValid(request,client,protocol,response,responseSize);

            return isResponseValid;
        }
    }
}
