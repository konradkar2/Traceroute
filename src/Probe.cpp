#include <Traceroute/Probe.hpp>
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
    mResponseSender = responseSender;
}
const SocketAddress &ProbeResultContainer::GetResponseAddr() const
{
    return mResponseSender;
}
const std::vector<ProbeResultContainer::ProbeResult> &ProbeResultContainer::getResults() const
{
    return mProbeResults;
}
std::string ProbeResultContainer::toString() const
{
    std::string result;
    result = std::to_string(mTtl) + "  " + mResponseSender.toString();
    for (size_t i = 0; i < mProbeResults.size(); i++)
    {
        ProbeResult pr = mProbeResults[i];
        if (pr.success)
        {
            std::string time = std::to_string(pr.receivedAfter.count() / 1000.0);
            time.erase(time.find_last_not_of('0') + 1, std::string::npos); // remove trailing zeroes
            result += "  " + time + " " + "ms";
        }
        else
        {
            result += " *";
        }
    }
    return result;
}

} // namespace traceroute