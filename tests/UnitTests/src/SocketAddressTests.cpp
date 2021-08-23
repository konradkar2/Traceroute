#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <Traceroute/SocketAddress.hpp>


TEST(SocketAddressTests, AddressIsParsedWithProperFamily4)
{
    const std::string address = "1.1.1.1";
    const auto expectedFamily = AF_INET;

    Traceroute::SocketAddress socketAddress{address};
    auto family = socketAddress.family();

    EXPECT_EQ(expectedFamily,family);
}
TEST(SocketAddressTests, AddressIsParsedWithProperFamily6)
{  
    const std::string address = "2001:0db8:0000:0000:0000:0000:1428:57ab";
    const auto expectedFamily = AF_INET6;

    Traceroute::SocketAddress socketAddress{address};
    auto family = socketAddress.family();

    EXPECT_EQ(expectedFamily,family);
}