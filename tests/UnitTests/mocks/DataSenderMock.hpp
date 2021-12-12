#pragma once

#include <Traceroute/interface/IDataSender.hpp>
#include <gmock/gmock.h>
#include <memory>

namespace traceroute {
class DataSenderMock : public IDataSender
{
  public:
    MOCK_METHOD(bool, trySending, (const std::unique_ptr<Packet> &, int, std::chrono::milliseconds), (override));
    MOCK_METHOD(std::optional<ResponseInfo>, tryReceiving, (char *, size_t, std::chrono::milliseconds), (override));
};
} // namespace traceroute