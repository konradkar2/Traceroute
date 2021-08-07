#pragma once
#include <chrono>
#include <Traceroute/Probe.hpp>
#include <Traceroute/Packet.hpp>
#include <Traceroute/ProbeSenderBase.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <Traceroute/IDataSender.hpp>
namespace Traceroute
{
    namespace Icmp
    {
        class IcmpProbeSender : public ProbeSender
        {
        public:
            IcmpProbeSender(IDataSender *dataSender);
            ProbeResultContainer SendProbe(const IcmpPacket *packet,
                                           int ttl, const int retries, std::chrono::microseconds timeout);

        protected:
            bool isResponseValid(const SocketAddress &client, const int &protocol) override;

        private:
            const IcmpPacket *mPacket;
        };
    }
}