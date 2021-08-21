#pragma once
#include <Traceroute/DataSenderBase.hpp>
#include <Traceroute/SocketAddress.hpp>
namespace Traceroute
{
    namespace DataSenders
    {
        class TcpDataSender : public DataSenderBase
        {
        public:
            TcpDataSender(int family, const SocketAddress &sourceAddr, int delayMs); //e.g IPPROTO_ICMP
            ~TcpDataSender(){};

        protected:
            int getSendingSocket() override;
            std::vector<SocketInfo> getReceivingSockets() override;

        private:
            int mSfdTcp;
        };
    }
}