#pragma once

#include <chrono>
#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/Probe.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/IDataSender.hpp>
#include <Traceroute/ProbeSenderBase.hpp>

namespace Traceroute
{
    namespace Udp
    {
        class UdpProbeSender : public ProbeSender
        {

        public:
            UdpProbeSender(IDataSender *dataSender);
            ~UdpProbeSender(){}; // TODO: Implement destructor
            ProbeResultContainer SendProbe(
                const UdpPacket *packet, int ttl, const int retries, std::chrono::microseconds timeout);

        protected:
            bool isResponseValid(const SocketAddress &client, const int &protocol) override;

        private:
            UdpProbeSender();
            const UdpPacket *mPacket;
        };
    }
}