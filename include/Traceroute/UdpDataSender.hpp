#include "NBlockDataSenderBase.hpp"
#include<netinet/in.h>
#include<sys/socket.h>
#include <chrono>
#include <thread>
#define SA (struct sockaddr*)
namespace Traceroute
{
    class UdpDataSender : public NBlockDataSenderBase
    {       
    public:
        UdpDataSender(int family, const SocketAddress & sourceAddr, int delayMs); //e.g IPPROTO_ICMP       
        ~UdpDataSender(){};
    protected:
        int GetSendingSocket() override;
        int GetReceivingSocket() override;  
        int GetCurrentProtocol() override;     
    private:      
        int _sfd_udp;    
    
    };
}