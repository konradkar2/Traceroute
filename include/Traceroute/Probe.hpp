#pragma once
#include <vector>
#include <string>
using namespace std;
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
        void Add(ProbeResult result);
        void SetResponseAddr(string responseAddr);
        const std::string & GetResponseAddr() const;
        
        string ToString();     
        const vector<ProbeResult> & GetResults() const;
    private:
        vector<ProbeResult> _probeResults;  
        string _responseAddr;   
        int _ttl;
    };
       
    typedef int ProtocolType;
    class TracerouteResultCointainer
    {
    public:
        TracerouteResultCointainer(const vector<ProbeResultContainer> & probeResults,
             ProtocolType protocol);
        string ResultsToString();
    private:
        ProtocolType _protocol;
        vector<ProbeResultContainer> & _probeResultsContainers;
    };

}