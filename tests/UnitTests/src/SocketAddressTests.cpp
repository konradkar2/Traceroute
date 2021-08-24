#include <gmock/gmock.h>
#include <string>
#include <vector>
#include <Traceroute/SocketAddress.hpp>
#include <sys/socket.h>

using ::testing::Eq;
using ::testing::IsTrue;
struct SocketAddressTests : public ::testing::Test
{
    Traceroute::SocketAddress socketAddress;
};

TEST_F(SocketAddressTests, addressIsRecognizedAsIPv4)
{
    const std::string address = "1.1.1.1";

    socketAddress = Traceroute::SocketAddress{address};

    EXPECT_THAT(socketAddress.isV4(), IsTrue);
}
TEST_F(SocketAddressTests, addressIsRecognizedAsIPv6)
{  
    const std::string address = "2001:0db8:0000:0000:0000:0000:1428:57ab";
    
    socketAddress = Traceroute::SocketAddress{address};

    EXPECT_THAT(socketAddress.isV6(), IsTrue);
}

TEST_F(SocketAddressTests, addressFamilyIsAF_INET)
{
    const std::string address = "1.1.1.1";

    socketAddress = Traceroute::SocketAddress{address};

    EXPECT_THAT(socketAddress.family(), Eq(AF_INET));
}
TEST_F(SocketAddressTests, addressFamilyIsAF_INET6)
{  
    const std::string address = "2001:0db8:0000:0000:0000:0000:1428:57ab";
    
    socketAddress = Traceroute::SocketAddress{address};

    EXPECT_THAT(socketAddress.family(), Eq(AF_INET6));
}
