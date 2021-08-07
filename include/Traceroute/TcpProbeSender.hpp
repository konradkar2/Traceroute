#pragma once
#include "Probe.hpp"
#include "Packet.hpp"
#include "IDataSender.hpp"
#include "ProbeSenderBase.hpp"
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>

namespace Traceroute
{
    class TcpProbeSender : public ProbeSenderBase
    {
    public:    
        TcpProbeSender(IDataSender * dataSender);
        ~TcpProbeSender(){}; // TODO: Implement destructor       
        ProbeResultContainer SendProbe(
            const TcpPacket * packet, int ttl, const int retries, std::chrono::microseconds timeout) ;       
    protected:        
        bool isResponseValid(const SocketAddress & client,const int & protocol) override;
    private:
         TcpProbeSender();         
        const TcpPacket * _packet;       
    
    };
}