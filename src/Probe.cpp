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

ProbeResultContainer::ProbeResultContainer(int ttl)
{
    mTtl = ttl;
}

void ProbeResultContainer::addFailedProbe(std::chrono::microseconds waitedFor)
{
    ProbeResult p;
    p.success   = false;
    p.waitedFor = waitedFor;
    mProbeResults.push_back(p);
}

void ProbeResultContainer::addSuccessfulProbe(std::chrono::microseconds waitedFor)
{
    ProbeResult p;
    p.success   = true;
    p.waitedFor = waitedFor;
    mProbeResults.push_back(p);
}
void ProbeResultContainer::setResponseAddr(const SocketAddress &responseSender)
{
    mResponseSender = responseSender;
}

std::optional<SocketAddress> ProbeResultContainer::GetResponseAddr() const
{
    return mResponseSender;
}

const std::vector<ProbeResultContainer::ProbeResult> &ProbeResultContainer::getResults() const
{
    return mProbeResults;
}

std::string ProbeResultContainer::toString() const
{
    std::stringstream result;
    std::string       client = mResponseSender.has_value() ? mResponseSender->toString() : "";
    result << std::to_string(mTtl) << "  " << client;
    for (size_t i = 0; i < mProbeResults.size(); i++)
    {
        ProbeResult pr = mProbeResults[i];
        if (pr.success)
        {
            std::string time = std::to_string(pr.waitedFor.count() / 1000.0);
            removeTrailingZeros(time);
            result << "  " + time << " "
                   << "ms";
        }
        else
        {
            result << " *";
        }
    }
    return result.str();
}

} // namespace traceroute