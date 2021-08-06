#pragma once
#include <cstdint>
#include <string>
#include<netinet/in.h>
#include<sys/socket.h>
#include<chrono>
#include<thread>
#include<netinet/icmp6.h>

#include "SocketAddress.hpp"
#include "IDataSender.hpp"
#define SA (struct sockaddr*)
namespace Traceroute
{
    class NBlockDataSenderBase : public IDataSender
    {
    public:       
        int SendTo(const char * buffer, size_t size, const SocketAddress & address) override;   
        //sets client ip addresss to 'address'
        //sets client transport protocol(or icmp/icmpv6) to 'protocol'
        int ReceiveFrom(char * buffer, size_t size, SocketAddress & address, int & protocol) override;   
        int GetReceiveDelayMs() override;
        void SetTtl(int ttl) override;      
    protected:       
        //binds address to sourceAddr
        //sets delay on socket (nonblocking)
        //sets protocol to transport layer of received packet(or icmp)
        NBlockDataSenderBase(int family, const SocketAddress & sourceAddr, int delayMs);
        virtual int getSendingSocket()=0;
        virtual int getReceivingSocket()=0;     
        virtual int getCurrentProtocol()=0;           
        int _sock_family;        
        int _sfd_icmp;
        int _delayMs;
    private:
        NBlockDataSenderBase();
        
        
    
    };
}