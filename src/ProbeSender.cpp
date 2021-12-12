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

namespace {
void handleValidResponse(const ResponseInfo &responseInfo, microseconds receivedAfter, ProbeResultContainer &output);
bool wasDestinationReached(const std::unique_ptr<Packet> &request, const ResponseInfo &responseInfo);
} // namespace

ProbeSender::ProbeSender(IPacketFactory &packetFactory, IDataSender &dataSender, std::shared_ptr<ISystemClock> clock)
    : mPacketFactory{packetFactory}, mDataSender{dataSender}, mSystemClock{clock}
{
}

std::vector<ProbeResultContainer> ProbeSender::beginProbing(int ttlBegin, int ttlEnd, int retries, microseconds timeout)
{
    bool                              destinationReached = false;
    std::vector<ProbeResultContainer> probesContainer;
    for (int ttl = ttlBegin; ttl <= ttlEnd; ++ttl)
    {
        ProbeResultContainer probes(ttl);
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
                    isResponseValid = packet->validate(respInfo.value(), mBuffer);
                }
            }
            if (isResponseValid)
            {
                probes.setResponseAddr(respInfo->client());
                probes.addSuccessfulProbe(getTimePassedTillNow(sentAt));
                if (respInfo->client() == packet->getDestinationAddress())
                {
                    destinationReached = true;
                }
            }
            else
            {
                probes.addFailedProbe(timeout);
            }
        }

        probesContainer.push_back(std::move(probes));
        if (destinationReached)
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

namespace {
void handleValidResponse(const ResponseInfo &responseInfo, microseconds receivedAfter, ProbeResultContainer &output)
{
    output.setResponseAddr(responseInfo.client());
    output.addSuccessfulProbe(receivedAfter);
}
bool wasDestinationReached(const std::unique_ptr<Packet> &request, const ResponseInfo &responseInfo)
{
    return request->getDestinationAddress() == responseInfo.client();
}

} // namespace

} // namespace traceroute