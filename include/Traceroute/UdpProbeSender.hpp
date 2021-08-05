#pragma once

#include "Probe.hpp"
#include "Packet.hpp"
#include "IDataSender.hpp"
#include "ProbeSenderBase.hpp"
#include <netinet/ip_icmp.h>
#include <netinet/icmp6.h>

namespace Traceroute
{
    class UdpProbeSender : public ProbeSenderBase
    {
    public:    
        UdpProbeSender(IDataSender * dataSender);
        ~UdpProbeSender(){}; // TODO: Implement destructor       
        ProbeResultContainer SendProbe(
            const UdpPacket * packet, int ttl, const int retries, const int timeoutms ) ;       
    protected:                 
        bool IsResponseValid(const SocketAddress & client,const int & protocol) override;
    private:
        UdpProbeSender();  
        const UdpPacket * _packet;       
    };
}