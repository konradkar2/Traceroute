#pragma once

#include <Traceroute/Packet.hpp>
#include <Traceroute/interface/IValidateResponse.hpp>
#include <gmock/gmock.h>

namespace traceroute {

const SocketAddress source{"1.2.3.4"};
const SocketAddress destination{"2.3.4.5"};

class PacketMock : public Packet
{
  public:
    PacketMock() : Packet(source, destination)
    {
    }
    MOCK_METHOD(std::string, serialize, (), (const, override));
    MOCK_METHOD(bool, isValid, (const ResponseInfo &, const char *), (override));
    virtual ~PacketMock() = default;
};

class PacketMockProxy : public Packet
{
  public:
    PacketMockProxy() : Packet(source, destination)
    {
    }
    void setMock(testing::NiceMock<PacketMock> *mock)
    {
        mMock = mock;
    }
    std::string serialize() const override
    {
        return mMock->serialize();
    }
    bool isValid(const ResponseInfo &responseInfo, const char *response) override
    {
        return mMock->isValid(responseInfo, response);
    }

  private:
    testing::NiceMock<PacketMock> *mMock;
};

} // namespace traceroute
