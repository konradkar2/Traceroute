#pragma once

#include <Traceroute/IDataSender.hpp>
#include <gmock/gmock.h>

namespace traceroute
{
class DataSenderMock : public IDataSender
{
  public:
    MOCK_METHOD(int, sendPacket, (const Packet &), (override));
    MOCK_METHOD(std::optional<ResponseInfo>, receiveFrom, (char *, size_t, std::chrono::milliseconds), (override));
    MOCK_METHOD(void, setTtlOnSendingSocket, (int), (override));
};
} // namespace traceroute