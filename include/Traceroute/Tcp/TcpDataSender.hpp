#pragma once
#include <Traceroute/NBlockDataSenderBase.hpp>
#include <Traceroute/SocketAddress.hpp>
namespace Traceroute
{
    namespace Tcp
    {
        class TcpDataSender : public NBlockDataSenderBase
        {
        public:
            TcpDataSender(int family, const SocketAddress &sourceAddr, int delayMs); //e.g IPPROTO_ICMP
            ~TcpDataSender(){};

        protected:
            int getSendingSocket() override;
            int getReceivingSocket() override;
            int getCurrentProtocol() override;

        private:
            void nextSocket();
            int mSfdTcp;
            int mCurrentSfd;
        };
    }
}