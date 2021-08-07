#include <Traceroute/ProbeSenderBase.hpp>
#include <stdexcept>
namespace Traceroute
{
    
    ProbeResultContainer ProbeSenderBase::BeginProbing(const Packet * packet,
            int ttl,int retries, std::chrono::microseconds timeout)
    {       
        mReceivingBuffer[BUFLEN] = {0};
        mSendingBuffer[BUFLEN] = {0};
        mPPacket = packet;
        setTtl(ttl);

        ProbeResultContainer resultContainer(getTtl());
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
                    ProbeResult r;
                    r.success = false;
                    r.receivedAfterTimeout = timeout;
                    resultContainer.add(r);
                    break;
                }

                int receivedn;
                SocketAddress client;                 
                int receivedProto; 
                receivedn = mPDataSender->receiveFrom(mReceivingBuffer,BUFLEN,client,receivedProto);
                if(receivedn >0 && isResponseValid(client,receivedProto))
                {
                    string responseAddr = client.toString();                                        
                    resultContainer.setResponseAddr(responseAddr);
                    auto now = chrono::steady_clock::now();
                    auto uspassed = chrono::duration_cast<chrono::microseconds>(now - start);
                    ProbeResult probeResult{true,uspassed};
                    resultContainer.add(probeResult);
                    break;
                }

                                 
                
            }           
        }  
        return resultContainer;
    }
    ProbeSenderBase::ProbeSenderBase(IDataSender * dataSender)
    {
        if(dataSender == nullptr)
        {
            throw std::invalid_argument("RawDataSender cannot be null");
        }
        mPDataSender = dataSender;      
    }
    void ProbeSenderBase::sendPacket()
    {
        mPPacket->serialize(mSendingBuffer);
        size_t packet_size = mPPacket->getSerializeSize();
        int sentn;
        sentn = mPDataSender->sendTo(mSendingBuffer,packet_size,mPPacket->getDestinationAddress());
    }
    char * ProbeSenderBase::getReceiveBuf()
    {
        return mReceivingBuffer;
    }
    int ProbeSenderBase::getTtl() const
    {
        return mTtl;
    }
    void ProbeSenderBase::setTtl(int ttl) 
    {
        mTtl = ttl;
        mPDataSender->setTtl(ttl);
    }    
    
}