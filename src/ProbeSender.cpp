#include <Traceroute/ProbeSender.hpp>
#include <stdexcept>
namespace Traceroute
{
    ProbeSender::ProbeSender(std::unique_ptr<IDataSender> dataSender, std::unique_ptr<IValidateResponse> responseValidator)
        : mDataSender{std::move(dataSender)}, mResponseValidator{std::move(responseValidator)}
    {
    }
    ProbeResultContainer ProbeSender::beginProbing(const Packet *packet,
                                                   int ttl, int retries, std::chrono::microseconds timeout)
    {
        mPacket = packet;
        mReceivingBuffer[BUFLEN] = {0};
        mSendingBuffer[BUFLEN] = {0};
        mDataSender->setTtlOnSocket(ttl);

        ProbeResultContainer resultContainer(ttl);
        for (int i = 0; i < retries; i++)
        {
            sendPacket();
            auto start = std::chrono::steady_clock::now();
            while (true)
            {
                auto end = std::chrono::steady_clock::now();
                auto timepassed = std::chrono::duration_cast<chrono::microseconds>(end - start);
                if (timepassed > timeout)
                {
                    resultContainer.addFailedProbe(timepassed);
                    break;
                }
                SocketAddress client;
                int responseProtocol;
                int responseSize = mDataSender->receiveFrom(mReceivingBuffer, BUFLEN, client, responseProtocol);
                auto isResponseValid = mResponseValidator->isResponseValid(*packet, client, responseProtocol, mReceivingBuffer, responseSize);
                if (isResponseValid)
                {
                    resultContainer.setResponseAddr(client);
                    auto now = chrono::steady_clock::now();
                    auto timepassed = chrono::duration_cast<chrono::microseconds>(now - start);
                    resultContainer.addSuccessfulProbe(timepassed);
                    break;
                }
            }
        }
        return resultContainer;
    }

    void ProbeSender::sendPacket()
    {
        mDataSender->sendTo(mPacket->serialize(), mPacket->getDestinationAddress());
    }
}