#include "common.hpp"
#include <Traceroute/ResponseValidators/ResponseValidator.hpp>
namespace traceroute::responseValidators {

bool ResponseValidator::validate(const ResponseInfo &responseInfo, const char *response)
{
    if (isIpHeaderPresent(responseInfo, response))
    {
        auto [newResponseInfo, newResponse] = skipIpHeader(responseInfo, response);
        return validateProtocol(newResponseInfo.protocol()) && validateSize(newResponseInfo.size()) &&
               validateFields(newResponseInfo, newResponse);
    }
    return validateProtocol(responseInfo.protocol()) && validateSize(responseInfo.size()) &&
           validateFields(responseInfo, response);
}

} // namespace traceroute::responseValidators