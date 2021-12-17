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

namespace traceroute {

class SequentialTraceroute : public Traceroute
{
  public:
    SequentialTraceroute(IPacketFactory &packetFactory, IDataSender &dataSender,
                         std::shared_ptr<ISystemClock> clock = std::make_shared<SystemClock>());
    ~SequentialTraceroute() = default;

    virtual std::vector<TracerouteResult> beginProbing(unsigned ttlBegin, unsigned ttlEnd, unsigned retries,
                                                       std::chrono::microseconds timeout) override;
};

} // namespace traceroute