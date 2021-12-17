#include "Traceroute/Probe.hpp"
#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/SequentialTraceroute.hpp>
#include <chrono>
#include <cstdio>
#include <memory>
#include <stdexcept>

namespace traceroute {

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;

SequentialTraceroute::SequentialTraceroute(IPacketFactory &packetFactory, IDataSender &dataSender,
                                           std::shared_ptr<ISystemClock> clock)
    : Traceroute(packetFactory, dataSender, clock)
{
}

std::vector<TracerouteResult> SequentialTraceroute::beginProbing(unsigned ttlBegin, unsigned ttlEnd, unsigned retries,
                                                                 microseconds timeout)
{
    bool                          destinationReached = false;
    std::vector<TracerouteResult> probes;
    for (unsigned ttl = ttlBegin; ttl <= ttlEnd; ++ttl)
    {
        std::vector<ProbeResult> probeResults;
        for (unsigned i = 0; i < retries; i++)
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
        probes.push_back(TracerouteResult{ttl, std::move(probeResults)});
        if (destinationReached)
        {
            break;
        }
    }
    return probes;
}

} // namespace traceroute