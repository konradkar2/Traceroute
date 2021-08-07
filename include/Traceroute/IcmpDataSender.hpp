#include "NBlockDataSenderBase.hpp"
#include "SocketAddress.hpp"



namespace Traceroute
{
    class IcmpDataSender : public NBlockDataSenderBase
    {       
    public:
        IcmpDataSender(int family, const SocketAddress & sourceAddr,int delayMs); //e.g IPPROTO_ICMP             
        ~IcmpDataSender(){};
    protected:
        int getSendingSocket() override;
        int getReceivingSocket() override;
        int getCurrentProtocol() override;
    };
}