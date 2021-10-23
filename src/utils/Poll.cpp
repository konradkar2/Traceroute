#include "Poll.hpp"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <poll.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <vector>

namespace traceroute::utils
{

int Poll(std::vector<pollfd> pollfds, std::chrono::milliseconds timeout)
{
    int success = poll(pollfds.data(), pollfds.size(), timeout.count());
    if (success < 0)
    {
        throw std::runtime_error("Exception occurred while polling " + std::string(strerror(errno)));
    }
    if (success > 0)
    {
        auto pollfd =
            std::find_if(begin(pollfds), end(pollfds), [](const auto &pollfd) { ; return pollfd.revents != 0; });
        if (pollfd != end(pollfds))
        {
            return pollfd->fd;
        }
    }
    return 0;
}
} // namespace traceroute::utils