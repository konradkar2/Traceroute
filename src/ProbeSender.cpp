#include <Traceroute/ProbeSender.hpp>
#include <stdexcept>
namespace Traceroute
{
    ProbeSender::ProbeSender(std::unique_ptr<IDataSender> && dataSender, std::unique_ptr<IValidateResponse> && responseValidator)
        :   mDataSender{std::move(dataSender)}, mResponseValidator{std::move(responseValidator)}
    {

    }
    ProbeResultContainer ProbeSender::beginProbing(const Packet * packet,
            int ttl,int retries, std::chrono::microseconds timeout)
    {       
        mPacket = packet;
        mReceivingBuffer[BUFLEN] = {0};
        mSendingBuffer[BUFLEN] = {0};       
        setTtl(ttl);

        ProbeResultContainer resultContainer(mTtl);
        for(int i = 0 ;i< retries ; i++)
        {
            
            sendPacket();            
            auto start = std::chrono::steady_clock::now();
            while(true)
            {
                auto end = std::chrono::steady_clock::now();
                auto uspassed = std::chrono::duration_cast<chrono::microseconds>(end - start);
                if(uspassed > timeout) //timeout
                {
                    ProbeResultContainer::ProbeResult r;
                    r.success = false;
                    r.receivedAfter = timeout;
                    resultContainer.add(r);
                    break;
                }

                
                SocketAddress client;                 
                int responseProtocol; 
                int responseSize = mDataSender->receiveFrom(mReceivingBuffer,BUFLEN,client,responseProtocol);

                
                auto isResponseValid = mResponseValidator->isResponseValid(*packet,client,responseProtocol,mReceivingBuffer,responseSize);
                if(isResponseValid)
                {
                    string responseAddr = client.toString();                                        
                    resultContainer.setResponseAddr(responseAddr);
                    auto now = chrono::steady_clock::now();
                    auto uspassed = chrono::duration_cast<chrono::microseconds>(now - start);                     
                    resultContainer.add(ProbeResultContainer::ProbeResult{true,uspassed});
                    break;
                }

                                 
                
            }           
        }  
        return resultContainer;
    }
    
    void ProbeSender::sendPacket()
    {
        mDataSender->sendTo(mPacket->serialize(),mPacket->getDestinationAddress());
    }
   
    void ProbeSender::setTtl(int ttl) 
    {
        mTtl = ttl;
        mDataSender->setTtl(ttl);
    }    
    
}