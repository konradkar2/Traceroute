
#include "Traceroute/DataSender.hpp"
#include "mocks/DataSenderMock.hpp"
#include "mocks/PacketFactoryMock.hpp"
#include "mocks/ResponseValidator.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace traceroute
{

struct ProbeSenderTest : public ::testing::Test
{
public:
    DataSenderMock dataSenderMock;
    PacketFactoryMock packetFactoryMock;
    ResponseValidatorMock responseValidatorMock;

    EXPECT_CALL(packetFactoryMock, createPacket(_))
};

} // namespace traceroute