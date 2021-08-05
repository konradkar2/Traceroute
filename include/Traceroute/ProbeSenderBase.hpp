#pragma once

#include "Probe.hpp"
#include "IDataSender.hpp"
#include <chrono>
#include "Packet.hpp"
namespace Traceroute
{    
    #define BUFLEN 4096
    class ProbeSenderBase
    {
    protected:  
        ProbeSenderBase(IDataSender * dataSender);       
        ProbeResultContainer BeginProbing(const Packet * packet,int ttl,const int retries, const int timeoutms );        
        
        virtual void SendPacket();
        virtual bool IsResponseValid(const SocketAddress & client, const int & protocol) = 0; 
        
        char * GetReceiveBuf();    
        int GetTtl() const;   
        virtual ~ProbeSenderBase(){};        
        
    private:
        void SetTtl(int ttl);
        const Packet * _packet;
        IDataSender * _dataSender;
        int _ttl;        
        char _sendBuf[BUFLEN] = { 0 };
        char _receiveBuf[BUFLEN] = { 0 };
        ProbeSenderBase();
    };

}