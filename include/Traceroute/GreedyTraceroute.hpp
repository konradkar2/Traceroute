#pragma once

#include "Packet.hpp"
#include "Probe.hpp"
#include "SystemClock.hpp"
#include "Traceroute/ResponseInfo.hpp"
#include "Traceroute/Traceroute.hpp"
#include "Traceroute/interface/ITraceroute.hpp"
#include <Traceroute/interface/IDataSender.hpp>
#include <Traceroute/interface/IPacketFactory.hpp>
#include <Traceroute/interface/ISystemClock.hpp>
#include <chrono>
#include <memory>
#include <mutex>
#include <optional>

namespace traceroute {

class GreedyTraceroute : public Traceroute
{
  public:
    GreedyTraceroute(IPacketFactory &packetFactory, IDataSender &dataSender,
                     std::shared_ptr<ISystemClock> clock = std::make_shared<SystemClock>());
    ~GreedyTraceroute() = default;

    virtual std::vector<TracerouteResult> beginProbing(unsigned ttlBegin, unsigned ttlEnd, unsigned retries,
                                                       std::chrono::microseconds timeout) override;

  private:
    std::vector<TracerouteResult> receiveAndValidate(size_t expectedSize, std::chrono::microseconds timeout);
    void                          sendPackets(unsigned ttlBegin, unsigned ttlEnd, unsigned retries);
    struct PacketToValidate
    {
        std::unique_ptr<Packet>               packet;
        std::chrono::steady_clock::time_point sentAt;
        unsigned                              ttl;
        unsigned                              retry;
    };
    std::vector<PacketToValidate> mPacketsToBeValidate;
    std::optional<SocketAddress>  mTarget;
    std::mutex                    mMutex;
};

} // namespace traceroute