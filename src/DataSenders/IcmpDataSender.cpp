#include <Traceroute/DataSenders/IcmpDataSender.hpp>
#include <Traceroute/DataSenderBase.hpp>
#include <vector>
namespace Traceroute
{
    namespace DataSenders
    {
        IcmpDataSender::IcmpDataSender(const SocketAddress &sourceAddr, std::chrono::milliseconds receiveTimeout)
        {
            mDataSenderBase = std::make_unique<DataSenderBase>(sourceAddr, SocketInfo{}, receiveTimeout);
        }

        int IcmpDataSender::sendTo(const std::string &&buffer, const SocketAddress &receiver)
        {
            return mDataSenderBase->sendTo(std::move(buffer), receiver);
        }

        int IcmpDataSender::receiveFrom(char *buffer, size_t size, SocketAddress &sender, int &protocol)
        {
            return mDataSenderBase->receiveFrom(buffer, size, sender, protocol);
        }

        void IcmpDataSender::setTtl(int ttl)
        {
            return mDataSenderBase->setTtl(ttl);
        }
    }

}