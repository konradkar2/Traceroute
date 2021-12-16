#include "Traceroute/Probe.hpp"
#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/ProbeSender.hpp>
#include <chrono>
#include <cstdio>
#include <memory>
#include <stdexcept>
namespace traceroute {

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;

ProbeSender::ProbeSender(IPacketFactory &packetFactory, IDataSender &dataSender, std::shared_ptr<ISystemClock> clock)
    : mPacketFactory{packetFactory}, mDataSender{dataSender}, mSystemClock{clock}
{
}

std::vector<TracerouteResult> ProbeSender::beginProbing(int ttlBegin, int ttlEnd, int retries, microseconds timeout)
{
    bool               destinationReached = false;
    std::vector<TracerouteResult> probes;
    for (int ttl = ttlBegin; ttl <= ttlEnd; ++ttl)
    {
        std::vector<ProbeResult> probeResults;
        for (int i = 0; i < retries; i++)
        {
            auto packet = mPacketFactory.createPacket();
            if (not mDataSender.trySending(packet, ttl, seconds(5)))
            {
                throw std::runtime_error("Failed to send a packet");
            }
            auto sentAt = mSystemClock->now();

            bool                        isResponseValid = false;
            std::optional<ResponseInfo> respInfo;
            while (true)
            {
                auto timeLeft = getTimeLeft(sentAt, timeout);
                if (isResponseValid || timeLeft < MinTimeWaitForResponse)
                {
                    break;
                }
                respInfo = mDataSender.tryReceiving(mBuffer, BufferSize, duration_cast<milliseconds>(timeLeft));
                if (respInfo)
                {
                    isResponseValid = packet->isValid(respInfo.value(), mBuffer);
                }
            }
            if (isResponseValid)
            {
                probeResults.push_back(successProbe(getTimePassedTillNow(sentAt), respInfo->client()));
                if (respInfo->client() == packet->getDestinationAddress())
                {
                    destinationReached = true;
                }
            }
            else
            {
                probeResults.push_back(failedProbe(timeout));
            }
        }
        probes.push_back(TracerouteResult{ttl,std::move(probeResults)});
        if (destinationReached)
        {
            break;
        }
    }
    return probes;
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