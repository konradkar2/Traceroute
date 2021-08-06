#include "NBlockDataSenderBase.hpp"
#include<netinet/in.h>
#include<sys/socket.h>
#include <chrono>
#include <thread>


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