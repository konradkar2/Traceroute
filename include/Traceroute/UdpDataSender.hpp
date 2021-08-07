#include "NBlockDataSenderBase.hpp"
#include "SocketAddress.hpp"

namespace Traceroute
{
    class UdpDataSender : public NBlockDataSenderBase
    {       
    public:
        UdpDataSender(int family, const SocketAddress & sourceAddr, int delayMs); //e.g IPPROTO_ICMP       
        ~UdpDataSender(){};
    protected:
        int getSendingSocket() override;
        int getReceivingSocket() override;  
        int getCurrentProtocol() override;     
    private:      
        int SfdUdp;    
    
    };
}