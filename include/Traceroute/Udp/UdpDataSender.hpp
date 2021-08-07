#pragma once
#include <Traceroute/NBlockDataSenderBase.hpp>
#include <Traceroute/SocketAddress.hpp>

namespace Traceroute
{
    namespace Udp
    {
        class UdpDataSender : public NBlockDataSenderBase
        {
        public:
            UdpDataSender(int family, const SocketAddress &sourceAddr, int delayMs); //e.g IPPROTO_ICMP
            ~UdpDataSender(){};

        protected:
            int getSendingSocket() override;
            int getReceivingSocket() override;
            int getCurrentProtocol() override;

        private:
            int SfdUdp;
        };
    }
}