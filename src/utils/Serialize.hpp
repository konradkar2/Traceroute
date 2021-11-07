#pragma once
#include <cstdint>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace traceroute::utils {

template <typename T, typename = std::enable_if<std::is_trivially_copyable_v<T>>>
std::string serialize(const T &pod)
{
    auto p = reinterpret_cast<const char *>(&pod);
    return std::string{p, p + sizeof(T)};
}

} // namespace traceroute::utils
