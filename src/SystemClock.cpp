#include <Traceroute/SystemClock.hpp>
#include <chrono>

namespace traceroute {

using namespace std::chrono;

steady_clock::time_point SystemClock::now()
{
    return steady_clock::now();
}


} // namespace traceroute