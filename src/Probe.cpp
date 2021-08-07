#include <Traceroute/Probe.hpp>
namespace Traceroute
{
    ProbeResultContainer::ProbeResultContainer(int ttl)
    {
        mTtl = ttl;
        MresponseAddr = "";
    }
    void ProbeResultContainer::add(ProbeResult result)
    {
        mProbeResults.push_back(result);
    }
    void ProbeResultContainer::setResponseAddr(const std::string & responseAddr)
    {
        MresponseAddr = responseAddr;
    }
    const std::string & ProbeResultContainer::GetResponseAddr() const 
    {
        return MresponseAddr;
    }
    const std::vector<ProbeResultContainer::ProbeResult> & ProbeResultContainer::getResults() const
    {
        return mProbeResults;
    }
    std::string ProbeResultContainer::toString()
    {
        std::string result;
        result = std::to_string(mTtl) + "  " + MresponseAddr;
        for(int i = 0; i< mProbeResults.size(); i++)
        {
            ProbeResult pr = mProbeResults[i];
            if(pr.success)
            {
                std::string time =  std::to_string(pr.receivedAfterTimeout.count() / 1000.0);
                time.erase ( time.find_last_not_of('0') + 1, std::string::npos ); //remove trailing zeroes
                result += "  " + time + " " + "ms";
            }
            else
            {
                result += " *";
            }
        }
        return result;
    }


}