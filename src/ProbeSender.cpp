#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/ProbeSender.hpp>
#include <chrono>
#include <cstdio>
#include <stdexcept>
namespace traceroute {

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

ProbeSender::ProbeSender(IPacketFactory &packetFactory, IDataSender &dataSender, std::shared_ptr<ISystemClock> clock)
    : mPacketFactory{packetFactory}, mDataSender{dataSender}, mSystemClock{clock}
{
}

std::vector<ProbeResultContainer> ProbeSender::beginProbing(int ttlBegin, int ttlEnd, int retries, microseconds timeout)
{
    bool                              wasDestinationReached = false;
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

            bool                        isResponseValid = false;
            std::optional<ResponseInfo> respInfo;
            while (true)
            {
                auto timeLeft = getTimeLeft(sendTimestamp, timeout);
                if (isResponseValid || timeLeft < MinTimeWaitForResponse)
                {
                    break;
                }
                respInfo = mDataSender.receiveFrom(mBuffer, BufferSize, duration_cast<milliseconds>(timeLeft));
                if (respInfo)
                {
                    isResponseValid = packet->validate(respInfo.value(), mBuffer);
                }
            }

            if (isResponseValid)
            {
                probes.setResponseAddr(respInfo->client());
                probes.addSuccessfulProbe(getTimePassedTillNow(sendTimestamp));
                if (respInfo->client() == packet->getDestinationAddress())
                {
                    wasDestinationReached = true;
                }
            }
            else
            {
                probes.addFailedProbe(timeout);
            }
        }
        probesContainer.push_back(std::move(probes));
        if (wasDestinationReached)
        {
            break;
        }
    }
    return probesContainer;
}

microseconds ProbeSender::getTimePassedTillNow(steady_clock::time_point then) const
{
    return duration_cast<microseconds>(mSystemClock->now() - then);
}
microseconds ProbeSender::getTimeLeft(steady_clock::time_point then, microseconds timeout) const
{
    auto timePassed = getTimePassedTillNow(then);
    if (timePassed > timeout)
        return 0us;
    return timeout - timePassed;
}

} // namespace traceroute