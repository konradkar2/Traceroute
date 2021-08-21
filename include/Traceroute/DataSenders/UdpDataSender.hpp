#pragma once
#include <Traceroute/DataSenderBase.hpp>
#include <Traceroute/SocketAddress.hpp>

namespace Traceroute
{
    namespace DataSenders
    {
        class UdpDataSender : public DataSenderBase
        {
        public:
            UdpDataSender(int family, const SocketAddress &sourceAddr, int delayMs); //e.g IPPROTO_ICMP
            ~UdpDataSender(){};

        protected:
            int getSendingSocket() override;
            std::vector<SocketInfo> getReceivingSockets() override;
        private:
            int SfdUdp;
        };
    }
}