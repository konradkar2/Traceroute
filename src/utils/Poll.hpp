#pragma once
#include <chrono>
#include <optional>
#include <poll.h>
#include <vector>

namespace traceroute::utils {

/// poll wrapper, returns a fd within given timeout
std::optional<int> Poll(const std::vector<int> &fds, short events, std::chrono::milliseconds timeout);
bool               Poll(int fd, short events, std::chrono::milliseconds timeout);

} // namespace traceroute::utils