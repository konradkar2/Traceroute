#include "Poll.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <poll.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <vector>

namespace traceroute::utils {

std::optional<int> Poll(const std::vector<int> &fds, short events, std::chrono::milliseconds timeout)
{
    std::vector<pollfd> pollfds{fds.size()};
    std::transform(begin(fds), end(fds), back_inserter(pollfds), [=](int fd) { return pollfd{fd, events, 0}; });
    int result = ::poll(pollfds.data(), pollfds.size(), timeout.count());
    if (result < 0)
    {
        throw std::runtime_error("Exception occurred while polling " + std::string(strerror(errno)));
    }
    if (result > 0)
    {
        auto pollfd =
            std::find_if(begin(pollfds), end(pollfds), [](const auto &pollfd) { return pollfd.revents != 0; });
        if (pollfd != end(pollfds))
        {
            return pollfd->fd;
        }
    }
    return std::nullopt;
}
bool Poll(int fd, short events, std::chrono::milliseconds timeout)
{
    pollfd pollFd{fd, events, 0};
    int    result = ::poll(&pollFd, 1, timeout.count());
    if (result < 0)
    {
        throw std::runtime_error("Exception occurred while polling " + std::string(strerror(errno)));
    }
    return static_cast<bool>(result);
}

} // namespace traceroute::utils