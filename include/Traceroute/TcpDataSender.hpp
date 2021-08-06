#include "NBlockDataSenderBase.hpp"
#include<netinet/in.h>
#include<sys/socket.h>
#include <chrono>
#include <thread>
#define SA (struct sockaddr*)
namespace Traceroute
{
    class TcpDataSender : public NBlockDataSenderBase
    {       
    public:
        TcpDataSender(int family, const SocketAddress & sourceAddr,int delayMs); //e.g IPPROTO_ICMP           
        ~TcpDataSender(){};
    protected:
        int getSendingSocket() override;
        int getReceivingSocket() override;       
        int getCurrentProtocol() override;
    private:      
        void nextSocket();
        int _sfd_tcp;
        int _sfd_current;
    };
}