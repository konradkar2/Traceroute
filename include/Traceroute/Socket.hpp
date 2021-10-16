#pragma once

namespace traceroute
{
struct Socket
{
    int sfd;
    int protocol;
    bool isSending;
    bool isReceiving;
};

} // namespace traceroute