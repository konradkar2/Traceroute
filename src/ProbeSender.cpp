#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/ProbeSender.hpp>
#include <chrono>
#include <cstdio>
#include <stdexcept>
namespace traceroute {

ProbeSender::ProbeSender(IPacketFactory &packetFactory, IDataSender &dataSender, IValidateResponse &responseValidator,
                         std::shared_ptr<ISystemClock> clock)
    : mPacketFactory{packetFactory}, mDataSender{dataSender}, mResponseValidator{responseValidator}, mSystemClock{clock}
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
            auto sendTimestamp = mSystemClock->now();

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

std::chrono::microseconds ProbeSender::getTimePassedTillNow(std::chrono::steady_clock::time_point then)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(mSystemClock->now() - then);
}
std::chrono::microseconds ProbeSender::getTimeLeft(std::chrono::steady_clock::time_point then,
                                                   std::chrono::microseconds timeout)
{
    auto timePassed = getTimePassedTillNow(then);
    if (timePassed > timeout)
        return 0us;
    return timeout - timePassed;
}

} // namespace traceroute