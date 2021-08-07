#pragma once
#include <chrono>
#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/Probe.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/IDataSender.hpp>
#include <Traceroute/ProbeSenderBase.hpp>

namespace Traceroute
{
    namespace Tcp
    {
        class TcpProbeSender : public ProbeSender
        {
        public:
            TcpProbeSender(IDataSender *dataSender);
            ~TcpProbeSender(){}; // TODO: Implement destructor
            ProbeResultContainer sendProbe(
                const TcpPacket *packet, int ttl, const int retries, std::chrono::microseconds timeout);

        protected:
            bool isResponseValid(const SocketAddress &client, const int &protocol) override;

        private:
            TcpProbeSender();
            const TcpPacket *mPacket;
        };
    }
}