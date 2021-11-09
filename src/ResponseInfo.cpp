#include <Traceroute/ResponseInfo.hpp>

namespace traceroute
{
ResponseInfo::ResponseInfo(SocketAddress &&client, int protocol, ssize_t size)
    : mClient{std::move(client)}, mProtocol{protocol}, mSize(size)
{
}
const SocketAddress &ResponseInfo::client() const
{
    return mClient;
}
int ResponseInfo::protocol() const
{
    return mProtocol;
}
size_t ResponseInfo::size() const
{
    return mSize;
}

} // namespace traceroute