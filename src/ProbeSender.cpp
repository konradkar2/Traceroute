#include <Traceroute/ProbeSender.hpp>
#include <stdexcept>
#include <chrono>
namespace traceroute
{
    namespace
    {
        std::chrono::microseconds getTimePassedTillNow(std::chrono::time_point<std::chrono::steady_clock> then);
    }
    ProbeSender::ProbeSender(std::unique_ptr<IPacketFactory> packetFactory,
                             std::unique_ptr<IDataSender> dataSender,
                             std::unique_ptr<IValidateResponse> responseValidator)
        : mPacketFactory{std::move(packetFactory)},
          mDataSender{std::move(dataSender)},
          mResponseValidator{std::move(responseValidator)}
    {
    }
    std::vector<ProbeResultContainer> ProbeSender::beginProbing(
        int ttlBegin, int ttlEnd, int retries, std::chrono::microseconds timeout)
    {
        bool wasDestinationReached = false;
        std::vector<ProbeResultContainer> probesContainer;
        for (int ttl = ttlBegin; ttl <= ttlEnd; ++ttl)
        {
            mDataSender->setTtlOnSendingSocket(ttl);

            ProbeResultContainer probes(ttl);
            for (int i = 0; i < retries; i++)
            {
                auto packet = mPacketFactory->createPacket();
                mDataSender->sendTo(packet->serialize(), packet->getDestinationAddress());
                auto sendTimestamp = std::chrono::steady_clock::now();
                while (true)
                {
                    if (getTimePassedTillNow(sendTimestamp) > timeout)
                    {
                        probes.addFailedProbe(timeout);
                        break;
                    }
                    SocketAddress client;
                    int responseProtocol;
                    int responseSize = mDataSender->receiveFrom(mReceivingBuffer, BUFLEN, client, responseProtocol);
                    auto isResponseValid = mResponseValidator->isResponseValid(*packet, client, responseProtocol, mReceivingBuffer, responseSize);
                    if (isResponseValid)
                    {
                        probes.setResponseAddr(client);
                        probes.addSuccessfulProbe(getTimePassedTillNow(sendTimestamp));
                        if (client == packet->getDestinationAddress())
                            wasDestinationReached = true;
                        break;
                    }
                }
            }
            probesContainer.push_back(std::move(probes));
            if (wasDestinationReached)
                break;
        }
        return probesContainer;
    }

    namespace
    {
        std::chrono::microseconds getTimePassedTillNow(std::chrono::time_point<std::chrono::steady_clock> then)
        {
            return chrono::duration_cast<chrono::microseconds>(std::chrono::steady_clock::now() - then);
        }
    } //namespace
}