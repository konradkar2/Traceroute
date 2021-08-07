#pragma once
#include <chrono>
#include "SocketAddress.hpp"
#include "Probe.hpp"
#include "Packet.hpp"
#include "IDataSender.hpp"
#include "ProbeSenderBase.hpp"


namespace Traceroute
{
    class TcpProbeSender : public ProbeSenderBase
    {
    public:    
        TcpProbeSender(IDataSender * dataSender);
        ~TcpProbeSender(){}; // TODO: Implement destructor       
        ProbeResultContainer sendProbe(
            const TcpPacket * packet, int ttl, const int retries, std::chrono::microseconds timeout) ;       
    protected:        
        bool isResponseValid(const SocketAddress & client,const int & protocol) override;
    private:
         TcpProbeSender();         
        const TcpPacket * mPacket;       
    
    };
}