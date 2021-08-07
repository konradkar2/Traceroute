#pragma once
#include <chrono>
#include "Probe.hpp"
#include "Packet.hpp"
#include "ProbeSenderBase.hpp"
#include "SocketAddress.hpp"
#include "IDataSender.hpp"
namespace Traceroute
{
    class IcmpProbeSender : public ProbeSenderBase
    {
    public:    
        IcmpProbeSender(IDataSender * dataSender);   
        ProbeResultContainer SendProbe(const IcmpPacket * packet,
            int ttl, const int retries, std::chrono::microseconds timeout );        
    protected:         
        bool isResponseValid(const SocketAddress & client, const int & protocol) override;
    private:        
        const IcmpPacket * mPacket;
    };
}