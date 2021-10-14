#include <Traceroute/DataSenderBase.hpp>
#include <chrono>
#include <cstdint>
#include <netinet/icmp6.h>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <sys/socket.h>
#include <thread>

int Poll(std::vector<int> fds, std::chrono::milliseconds timeout)
{
    
}