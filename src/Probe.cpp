#include <Traceroute/Probe.hpp>
#include <memory>
#include <sstream>

namespace traceroute
{
ProbeResultContainer::ProbeResultContainer(int ttl)
{
    mTtl = ttl;
}
void ProbeResultContainer::addFailedProbe(std::chrono::microseconds waitedFor)
{
    ProbeResult p;
    p.success = false;
    p.receivedAfter = waitedFor;
    mProbeResults.push_back(p);
}
void ProbeResultContainer::addSuccessfulProbe(std::chrono::microseconds waitedFor)
{
    ProbeResult p;
    p.success = true;
    p.receivedAfter = waitedFor;
    mProbeResults.push_back(p);
}
void ProbeResultContainer::setResponseAddr(const SocketAddress &responseSender)
{
    mResponseSender = std::make_unique<SocketAddress>(responseSender);
}

const std::vector<ProbeResultContainer::ProbeResult> &ProbeResultContainer::getResults() const
{
    return mProbeResults;
}
std::string ProbeResultContainer::toString() const
{
    std::stringstream result;
    std::string client = mResponseSender != nullptr ? mResponseSender->toString() : "";
    result << std::to_string(mTtl) << "  " << client;
    for (size_t i = 0; i < mProbeResults.size(); i++)
    {
        ProbeResult pr = mProbeResults[i];
        if (pr.success)
        {
            std::string time = std::to_string(pr.receivedAfter.count() / 1000.0);
            time.erase(time.find_last_not_of('0') + 1, std::string::npos); // remove trailing zeroes
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