#include "Traceroute/Traceroute.hpp"
#include <chrono>
#include <cstdio>
#include <memory>

namespace traceroute {

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;

Traceroute::Traceroute(IPacketFactory &packetFactory, IDataSender &dataSender, std::shared_ptr<ISystemClock> clock)
    : mPacketFactory{packetFactory}, mDataSender{dataSender}, mSystemClock{clock}
{
}

microseconds Traceroute::getTimePassedTillNow(steady_clock::time_point then) const
{
    return duration_cast<microseconds>(mSystemClock->now() - then);
}
microseconds Traceroute::getTimeLeft(steady_clock::time_point then, microseconds timeout) const
{
    auto timePassed = getTimePassedTillNow(then);
    if (timePassed > timeout)
        return 0us;
    return timeout - timePassed;
}

} // namespace traceroute