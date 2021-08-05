#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <Traceroute/SocketAddress.hpp>

TEST(StringExtensionsTests, testWorking) {
    ASSERT_EQ(false,false);
}


TEST(SocketAddressTests, TextCtorFamily)
{
    std::string adressV4 = "1.1.1.1";

    Traceroute::SocketAddress socketAddress4{adressV4};
    auto family = socketAddress4.GetFamily();

    EXPECT_EQ(family,AF_INET);

    std::string adressV6 = "2001:0db8:0000:0000:0000:0000:1428:57ab";

    Traceroute::SocketAddress socketAddress6{adressV6};
    family = socketAddress6.GetFamily();

    EXPECT_EQ(family,AF_INET6);
}