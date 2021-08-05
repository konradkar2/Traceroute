#include <Traceroute/ProbeSenderBase.hpp>
namespace Traceroute
{
    
    ProbeResultContainer ProbeSenderBase::BeginProbing(const Packet * packet,
            int ttl,const int retries, const int timeoutms)
    {       
        _receiveBuf[BUFLEN] = {0};
        _sendBuf[BUFLEN] = {0};
        _packet = packet;
        SetTtl(ttl);

        ProbeResultContainer resultContainer(GetTtl());
        for(int i = 0 ;i< retries ; i++)
        {
            
            SendPacket();            
            auto start = chrono::steady_clock::now();
            while(true)
            {
                auto end = chrono::steady_clock::now();
                int uspassed = chrono::duration_cast<chrono::microseconds>(end - start).count();
                if(uspassed > (timeoutms * 1000)) //timeout
                {
                    ProbeResult r;
                    r.success = false;
                    r.timevalms = timeoutms;
                    resultContainer.Add(r);
                    break;
                }

                int receivedn;
                SocketAddress client;                 
                int receivedProto; 
                receivedn = _dataSender->ReceiveFrom(_receiveBuf,BUFLEN,client,receivedProto);
                if(receivedn >0 && IsResponseValid(client,receivedProto))
                {
                    string responseAddr = client.ToString();                                        
                    resultContainer.SetResponseAddr(responseAddr);
                    auto now = chrono::steady_clock::now();
                    int uspassed = chrono::duration_cast<chrono::microseconds>(now - start).count();
                    ProbeResult r;
                    r.success = true;
                    r.timevalms = (double)uspassed/1000.0;
                    resultContainer.Add(r);
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
        _dataSender = dataSender;      
    }
    void ProbeSenderBase::SendPacket()
    {
        _packet->Serialize(_sendBuf);
        size_t packet_size = _packet->Serialize_size();
        int sentn;
        sentn = _dataSender->SendTo(_sendBuf,packet_size,_packet->GetDestinationAddress());
    }
    char * ProbeSenderBase::GetReceiveBuf()
    {
        return _receiveBuf;
    }
    int ProbeSenderBase::GetTtl() const
    {
        return _ttl;
    }
    void ProbeSenderBase::SetTtl(int ttl) 
    {
        _ttl = ttl;
        _dataSender->SetTtl(ttl);
    }    
    
}