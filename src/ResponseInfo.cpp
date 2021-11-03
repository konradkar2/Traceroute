#include <Traceroute/ResponseInfo.hpp>

namespace traceroute
{
ResponseInfo::ResponseInfo(SocketAddress &&client, int protocol, ssize_t size)
    : mClient{std::move(client)}, mProtocol{protocol}, mSize(size)
{
}
const SocketAddress &ResponseInfo::client()
{
    return mClient;
}
int ResponseInfo::protocol()
{
    return mProtocol;
}
ssize_t ResponseInfo::size()
{
    return mSize;
}

} // namespace traceroute