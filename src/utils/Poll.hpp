#pragma once
#include <chrono>
#include <optional>
#include <poll.h>
#include <vector>

namespace traceroute::utils {

///poll wrapper, returns a fd within given timeout
std::optional<int> Poll(std::vector<pollfd> pollfs, std::chrono::milliseconds timeout);

} // namespace traceroute::utils