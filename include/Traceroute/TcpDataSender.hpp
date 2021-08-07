#include "NBlockDataSenderBase.hpp"

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
        int mSfdTcp;
        int mCurrentSfd;
    };
}