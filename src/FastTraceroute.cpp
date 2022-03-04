#include "Traceroute/Probe.hpp"
#include "Traceroute/ResponseInfo.hpp"
#include <Traceroute/FastTraceroute.hpp>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <future>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <stdexcept>

namespace traceroute {

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::milliseconds;
using std::chrono::seconds;


FastTraceroute::FastTraceroute(IPacketFactory &packetFactory, IDataSender &dataSender,
                                   std::shared_ptr<ISystemClock> clock)
    : Traceroute(packetFactory, dataSender, clock)
{
}

std::vector<TracerouteResult> FastTraceroute::beginProbing(unsigned ttlBegin, unsigned ttlEnd, unsigned retries,
                                                             microseconds timeout)
{
    auto sendHandle = std::async(std::launch::async, &FastTraceroute::sendPackets, this, ttlBegin, ttlEnd, retries);
    const size_t expectedSize = (ttlEnd - ttlBegin + 1) * retries;
    return receiveAndValidate(expectedSize, timeout);
}

void FastTraceroute::sendPackets(unsigned ttlBegin, unsigned ttlEnd, unsigned retries)
{
    for (unsigned ttl = ttlBegin; ttl <= ttlEnd; ++ttl)
    {
        for (unsigned i = 0; i < retries; ++i)
        {
            auto packet = mPacketFactory.createPacket();
            if (not mDataSender.trySending(packet, ttl, seconds(5)))
            {
                throw std::runtime_error("Failed to send a packet");
            }
            std::lock_guard lock(mMutex);
            mPacketsToBeValidated.push_back(PacketToValidate{std::move(packet), mSystemClock->now(), ttl, i});
        }
    }
}

std::vector<TracerouteResult> FastTraceroute::receiveAndValidate(size_t expectedSize, microseconds timeout)
{
    std::map<unsigned, std::vector<ProbeResult>> ttlToProbeResults;

    auto getCurrentSize = [&]() {
        return std::accumulate(begin(ttlToProbeResults), end(ttlToProbeResults), 0u,
                               [](unsigned total, const auto &kv) { return total += kv.second.size(); });
    };

    const auto expectedMaxTimeTaken = duration_cast<milliseconds>(timeout) * expectedSize;

    auto beginAt = mSystemClock->now();
    while (getCurrentSize() < expectedSize and getTimeLeft(beginAt, expectedMaxTimeTaken) > 0us)
    {
        std::optional<ResponseInfo> respInfo =
            mDataSender.tryReceiving(mBuffer, BufferSize, duration_cast<milliseconds>(timeout));
        {
            std::lock_guard lock(mMutex);
            if (respInfo)
            {
                auto validPacketIt = std::find_if(begin(mPacketsToBeValidated), end(mPacketsToBeValidated),
                                                  [&](const PacketToValidate &packetTV) {
                                                      return packetTV.packet->isValid(respInfo.value(), mBuffer);
                                                  });
                if (validPacketIt != end(mPacketsToBeValidated))
                {
                    ttlToProbeResults[validPacketIt->ttl].push_back(
                        successProbe(getTimePassedTillNow(validPacketIt->sentAt), respInfo->client()));
                    mPacketsToBeValidated.erase(validPacketIt);
                }
            }
            for (auto it = begin(mPacketsToBeValidated); it != end(mPacketsToBeValidated);)
            {
                const auto timeLeft = getTimeLeft(it->sentAt, timeout);
                if (timeLeft < MinTimeWaitForResponse)
                {
                    ttlToProbeResults[it->ttl].push_back(failedProbe(getTimePassedTillNow(it->sentAt)));
                    mPacketsToBeValidated.erase(it);
                }
                else
                    ++it;
            }
        }
    }
    std::vector<TracerouteResult> results;
    std::transform(begin(ttlToProbeResults), end(ttlToProbeResults), back_inserter(results),
                   [](auto &kv) { return TracerouteResult(kv.first, std::move(kv.second)); });
    std::sort(begin(results), end(results), [](const auto &trA, const auto &trB) { return trA.ttl() < trB.ttl(); });

    auto targetAddress = mPacketFactory.createPacket()->getDestinationAddress();
    auto targetIt      = std::find_if(begin(results), end(results), [&](const TracerouteResult &tr) {
        return std::any_of(begin(tr.probeResults()), end(tr.probeResults()), [&](const ProbeResult &prResult) {
            return prResult.responseSender().has_value() and prResult.responseSender() == targetAddress;
             });
         });

    if (std::distance(targetIt, end(results)) > 0)
        results.erase(targetIt + 1, end(results));
    return results;
}

} // namespace traceroute