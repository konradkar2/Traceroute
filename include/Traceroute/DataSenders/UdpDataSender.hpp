#pragma once
#include <Traceroute/IDataSender.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <memory>
#include <chrono>

namespace Traceroute
{
    namespace DataSenders
    {
        class UdpDataSender : public IDataSender
        {
        public:
            UdpDataSender(const SocketAddress &sourceAddr, std::chrono::milliseconds receiveTimeout); //e.g IPPROTO_ICMP
            ~UdpDataSender(){};
            int sendTo(const std::string &&buffer, const SocketAddress &receiver) override;
            int receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol) override;
            void setTtlOnSocket(int ttl) override;
            
        private:
            std::unique_ptr<IDataSender> mDataSenderBase;
        };
    }
}