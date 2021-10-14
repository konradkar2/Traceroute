#include <Traceroute/ProbeSender.hpp>
#include <chrono>
#include <stdexcept>
namespace traceroute
{
namespace
{
std::chrono::microseconds getTimePassedTillNow(std::chrono::time_point<std::chrono::steady_clock> then);
}
ProbeSender::ProbeSender(std::unique_ptr<IPacketFactory> packetFactory, std::unique_ptr<IDataSender> dataSender,
                         std::unique_ptr<IValidateResponse> responseValidator)
    : mPacketFactory{std::move(packetFactory)}, mDataSender{std::move(dataSender)}, mResponseValidator{
                                                                                        std::move(responseValidator)}
{
}

std::vector<ProbeResultContainer> ProbeSender::beginProbing(int ttlBegin, int ttlEnd, int retries,
                                                            std::chrono::microseconds timeout)
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

            SocketAddress client;
            int responseProtocol;
            bool isResponseValid = false;
            int responseSize;
            while (not (isResponseValid || getTimePassedTillNow(sendTimestamp) > timeout))
            {
                responseSize = mDataSender->receiveFrom(mBuffer, BUFLEN, client, responseProtocol);
                isResponseValid =
                    mResponseValidator->validate(*packet, client, responseProtocol, mBuffer, responseSize);
            }
            if (isResponseValid)
            {
                probes.setResponseAddr(client);
                probes.addSuccessfulProbe(getTimePassedTillNow(sendTimestamp));
                if (client == packet->getDestinationAddress())
                    wasDestinationReached = true;
            }
            else
            {
                probes.addFailedProbe(timeout);
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
} // namespace
} // namespace traceroute