#include <Traceroute/Probe.hpp>
namespace Traceroute
{
    ProbeResultContainer::ProbeResultContainer(int ttl)
    {
        _ttl = ttl;
        _responseAddr = "";
    }
    void ProbeResultContainer::Add(ProbeResult result)
    {
        _probeResults.push_back(result);
    }
    void ProbeResultContainer::SetResponseAddr(string responseAddr)
    {
        _responseAddr = responseAddr;
    }
    string ProbeResultContainer::GetResponseAddr()
    {
        return _responseAddr;
    }
    const vector<ProbeResult> & ProbeResultContainer::GetResults()
    {
        return _probeResults;
    }
    string ProbeResultContainer::ToString()
    {
        string result;
        result = std::to_string(_ttl) + "  " + _responseAddr;
        for(int i = 0; i< _probeResults.size(); i++)
        {
            ProbeResult pr = _probeResults[i];
            if(pr.success)
            {
                string time = std::to_string(pr.timevalms);
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