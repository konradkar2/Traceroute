#pragma once
#include <Traceroute/NBlockDataSenderBase.hpp>
#include <Traceroute/SocketAddress.hpp>

namespace Traceroute
{
    namespace DataSenders

    {
        class IcmpDataSender : public NBlockDataSenderBase
        {
        public:
            IcmpDataSender(int family, const SocketAddress &sourceAddr, int delayMs); //e.g IPPROTO_ICMP
            ~IcmpDataSender(){};

        protected:
            int getSendingSocket() override;
            int getReceivingSocket() override;
            int getCurrentProtocol() override;
        };
    } 
}