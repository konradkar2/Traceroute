#pragma once
#include <vector>
#include <string>
#include <chrono>
namespace Traceroute
{
    

    class ProbeResultContainer
    {
    public:   
        struct ProbeResult
        {   
            bool success;
            std::chrono::microseconds receivedAfter;
        };
        ProbeResultContainer(int ttl);
        void add(ProbeResult result);
        void setResponseAddr(const std::string & responseAddr);
        const std::string & GetResponseAddr() const;
        
        std::string toString() const;
        const std::vector<ProbeResult> & getResults() const;
    private:
        std::vector<ProbeResult> mProbeResults;  
        std::string MresponseAddr;   
        int mTtl;
    };
       
    


}