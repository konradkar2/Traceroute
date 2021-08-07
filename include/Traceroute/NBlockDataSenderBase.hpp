#pragma once

#include <cstdint>
#include "SocketAddress.hpp"
#include "IDataSender.hpp"

namespace Traceroute
{
    class NBlockDataSenderBase : public IDataSender
    {
    public:       
        int sendTo(const char * buffer, size_t size, const SocketAddress & address) override;   
        //sets client ip addresss to 'address'
        //sets client transport protocol(or icmp/icmpv6) to 'protocol'
        int receiveFrom(char * buffer, size_t size, SocketAddress & address, int & protocol) override;   
        int getReceiveDelayMs() override;
        void setTtl(int ttl) override;      
    protected:       
        //binds address to sourceAddr
        //sets delay on socket (nonblocking)
        //sets protocol to transport layer of received packet(or icmp)
        NBlockDataSenderBase(int family, const SocketAddress & sourceAddr, int delayMs);
        virtual int getSendingSocket()=0;
        virtual int getReceivingSocket()=0;     
        virtual int getCurrentProtocol()=0;           
        int mSock_family;        
        int mSfd_icmp;
        int mDelayMs;
    private:
        NBlockDataSenderBase();
        
        
    
    };
}