#pragma once

#include <cstdint>
#include <type_traits>

namespace traceroute {

struct Socket
{
    int fd;
    int protocol;
};

enum class Role : std::uint8_t
{
    Receive = 1,
    Send = 2
};

struct SocketExt
{
    Socket socket;
    Role role;
};

inline Role operator|(Role lhs, Role rhs)
{
    return static_cast<Role>(static_cast<std::underlying_type_t<Role>>(lhs) |
                             static_cast<std::underlying_type_t<Role>>(rhs));
}

inline bool operator&(Role lhs, Role rhs)
{
    return static_cast<bool>(static_cast<std::underlying_type_t<Role>>(lhs) &
                             static_cast<std::underlying_type_t<Role>>(rhs));
}

} // namespace traceroute