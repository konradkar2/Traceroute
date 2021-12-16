#include "common.hpp"
#include <Traceroute/ResponseValidators/ResponseValidator.hpp>
namespace traceroute::responseValidators {

bool ResponseValidator::isValid(const ResponseInfo &responseInfo, const char *response)
{
    auto validateAll = [this](const ResponseInfo &respInfo, const char *resp) {
        return validateProtocol(respInfo.protocol()) && validateSize(respInfo.size()) && validateFields(respInfo, resp);
    };

    if (isIpHeaderPresent(responseInfo, response))
    {
        auto [newResponseInfo, newResponse] = skipIpHeader(responseInfo, response);
        return validateAll(newResponseInfo, newResponse);
    }
    return validateAll(responseInfo, response);
}

} // namespace traceroute::responseValidators