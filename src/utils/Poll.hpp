#pragma once
#include <chrono>
#include <vector>

namespace traceroute::utils
{
int Poll(std::vector<int> fds, short int events, std::chrono::milliseconds timeout);
} // namespace traceroute::utils