#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/ProbeSender.hpp>
#include <chrono>
#include <cstdio>
#include <stdexcept>
namespace traceroute
{
using namespace std::chrono_literals;
namespace
{
std::chrono::microseconds getTimePassedTillNow(std::chrono::time_point<std::chrono::steady_clock> then);
std::chrono::microseconds getTimeLeft(std::chrono::time_point<std::chrono::steady_clock> then,
                                      std::chrono::microseconds timeout);
} // namespace
ProbeSender::ProbeSender(IPacketFactory &packetFactory, IDataSender &dataSender, IValidateResponse &responseValidator)
    : mPacketFactory{packetFactory}, mDataSender{dataSender}, mResponseValidator{responseValidator}
{
}

std::vector<ProbeResultContainer> ProbeSender::beginProbing(int ttlBegin, int ttlEnd, int retries,
                                                            std::chrono::microseconds timeout)
{
    bool wasDestinationReached = false;
    std::vector<ProbeResultContainer> probesContainer;
    for (int ttl = ttlBegin; ttl <= ttlEnd; ++ttl)
    {
        mDataSender.setTtlOnSendingSocket(ttl);

        ProbeResultContainer probes(ttl);
        for (int i = 0; i < retries; i++)
        {
            auto packet = mPacketFactory.createPacket();

            mDataSender.sendPacket(*packet);
            auto sendTimestamp = std::chrono::steady_clock::now();

            bool isResponseValid = false;
            std::optional<ResponseInfo> respInfo;
            while (true)
            {
                auto timeLeft = getTimeLeft(sendTimestamp, timeout);
                if (isResponseValid || timeLeft < MinTimeWaitForResponse)
                {
                    break;
                }
                respInfo = mDataSender.receiveFrom(mBuffer, BufferSize,
                                                   std::chrono::duration_cast<std::chrono::milliseconds>(timeLeft));
                if (respInfo)
                {
                    isResponseValid = mResponseValidator.validate(*packet, respInfo->client(), respInfo->protocol(),
                                                                  mBuffer, respInfo->size());
                }
            }

            if (isResponseValid)
            {
                probes.setResponseAddr(respInfo->client());
                probes.addSuccessfulProbe(getTimePassedTillNow(sendTimestamp));
                if (respInfo->client() == packet->getDestinationAddress())
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
std::chrono::microseconds getTimeLeft(std::chrono::time_point<std::chrono::steady_clock> then,
                                      std::chrono::microseconds timeout)
{
    auto timePassed = getTimePassedTillNow(then);
    if (timePassed > timeout)
        return 0us;
    return timeout - timePassed;
}

} // namespace
} // namespace traceroute