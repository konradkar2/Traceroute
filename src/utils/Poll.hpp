#pragma once
#include <chrono>
#include <vector>
#include <poll.h>

namespace traceroute::utils
{
int Poll(std::vector<pollfd> pollfs, std::chrono::milliseconds timeout);
} // namespace traceroute::utils