#include <Traceroute/Icmp/IcmpResponseValidator.hpp>
#include "V4/Icmp4ResponseValidator.hpp"
#include "V6/Icmp6ResponseValidator.hpp"

namespace Traceroute
{
    namespace Icmp
    {
        bool IcmpResponseValidator::isResponseValid(const Packet &request, const SocketAddress &client,
                                                    int protocol, const char *response, size_t responseSize)
        {
            bool isResponseValid = false;
            if(client.getFamily() == AF_INET)
                isResponseValid = V4::Icmp4ResponseValidator::instance().isResponseValid(request,client,protocol,response,responseSize);
            else
                isResponseValid = V6::Icmp6ResponseValidator::instance().isResponseValid(request,client,protocol,response,responseSize);

            return isResponseValid;
        }
    }
}
