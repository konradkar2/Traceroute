#pragma once
#include <vector>
#include <string>

namespace Traceroute
{
    struct ProbeResult
    {    
        bool success;
        double timevalms;
    };

    class ProbeResultContainer
    {
    public:   
        ProbeResultContainer(int ttl);
        void add(ProbeResult result);
        void setResponseAddr(const std::string & responseAddr);
        const std::string & GetResponseAddr() const;
        
        std::string toString();     
        const std::vector<ProbeResult> & getResults() const;
    private:
        std::vector<ProbeResult> mProbeResults;  
        std::string MresponseAddr;   
        int mTtl;
    };
       
    
    class TracerouteResultCointainer
    {
    public:
        using ProtocolType = int;
        TracerouteResultCointainer(const std::vector<ProbeResultContainer> & probeResults,
             ProtocolType protocol);
        std::string ResultsToString();
    private:
        ProtocolType mProtocol;
        std::vector<ProbeResultContainer> & mProbeResultsContainers;
    };

}