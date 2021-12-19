#include <Traceroute/Probe.hpp>
#include <memory>
#include <sstream>

namespace traceroute {
namespace {
void removeTrailingZeros(std::string &str)
{
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
}
} // namespace


ProbeResult failedProbe(std::chrono::microseconds waitedFor)
{
    return ProbeResult{false,waitedFor,std::nullopt};
}

ProbeResult successProbe(std::chrono::microseconds waitedFor,const SocketAddress &responseSender )
{
    return ProbeResult{true,waitedFor, responseSender};
}

std::string toString(const TracerouteResult & tracerouteResult)
{
    std::stringstream result;
    result << std::to_string(tracerouteResult.ttl()) << "  ";
    for(const ProbeResult & pr : tracerouteResult.probeResults())
    {
        if (pr.success())
        {
            result << (pr.responseSender().has_value() ? pr.responseSender()->toString() : "");
            std::string waitedForStr = std::to_string(pr.waitedFor().count() / 1000.0);
            removeTrailingZeros(waitedForStr);
            result << "  " + waitedForStr << " "
                   << "ms";
        }
        else
        {
            result << " *";
        }
        result << "\n";
    }
    return result.str();
}

} // namespace traceroute