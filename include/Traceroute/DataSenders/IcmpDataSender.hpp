#pragma once
#include <Traceroute/DataSenderBase.hpp>
#include <Traceroute/SocketAddress.hpp>

namespace Traceroute
{
    namespace DataSenders

    {
        class IcmpDataSender : public DataSenderBase
        {
        public:
            IcmpDataSender(int family, const SocketAddress &sourceAddr, int delayMs); //e.g IPPROTO_ICMP
            ~IcmpDataSender(){};

        protected:
            int getSendingSocket() override;
            std::vector<SocketInfo> getReceivingSockets() override;
        };
    } 
}