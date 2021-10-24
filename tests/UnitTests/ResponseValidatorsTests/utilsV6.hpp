#include <stdint.h>
#include <Traceroute/HeaderTypes.hpp>
#include <tuple>

namespace traceroute
{

template <typename ResponseV6>
std::tuple<const char *, size_t> responseV6ToPtr(const ResponseV6 *response)
{
    const char *resp = reinterpret_cast<const char *>(response);
    resp += sizeof(Ipv6Header);

    return {resp, sizeof(ResponseV6) - sizeof(Ipv6Header)};
}
} // namespace traceroute