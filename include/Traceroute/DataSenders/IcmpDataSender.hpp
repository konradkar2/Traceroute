#pragma once
#include <Traceroute/IDataSender.hpp>
#include <Traceroute/SocketAddress.hpp>
#include <memory>
#include <chrono>

namespace traceroute
{
    namespace DataSenders

    {
        class IcmpDataSender : public IDataSender
        {
        public:
            IcmpDataSender(const SocketAddress &sourceAddr, std::chrono::milliseconds receiveTimeout); 
            int sendTo(const std::string &&buffer, const SocketAddress &receiver) override;
            int receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol) override;
            void setTtlOnSendingSocket(int ttl) override;
            ~IcmpDataSender(){};

        private:
            std::unique_ptr<IDataSender> mDataSenderBase;

        };
    }
}