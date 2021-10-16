#include "Poll.hpp"
#include <chrono>
#include <cstdint>
#include <poll.h>
#include <stdexcept>
#include <string.h>
#include <string>
#include <vector>

namespace traceroute::utils
{

int Poll(std::vector<int> fds, short int events, std::chrono::milliseconds timeout)
{
    std::vector<pollfd> pollds(fds.size());
    for (int fd : fds)
    {
        pollfd pfd;
        pfd.fd = fd;
        pfd.events = events;
        pollds.push_back(pfd);
    }

    int success = poll(pollds.data(), pollds.size(), timeout.count());
    if (success < 0)
    {
        throw std::runtime_error("Exception occurred while polling " + std::string(strerror(errno)));
    }
    if (success > 0)
    {
        for (const auto &pollfd : pollds)
        {
            if (pollfd.revents == 0)
                continue;
            else
            {
                return pollfd.fd;
            }
        }
    }
    return 0;
}
} // namespace traceroute::utils