#include <gmock/gmock.h>
#include <string>
#include <vector>
#include <Traceroute/SocketAddress.hpp>
#include <sys/socket.h>

using ::testing::Eq;
using ::testing::IsTrue;
using ::testing::Ne;

namespace traceroute
{

    TEST(SocketAddressTestV4,StringIsRecognizedAsIPv4)
    {
        const std::string address = "1.1.1.1";

        SocketAddress socketAddress{address};

        EXPECT_THAT(socketAddress.isV4(), IsTrue);
    }
    TEST(SocketAddressTestV4, AddressFamilyIsAF_INET)
    {
        const std::string address = "1.1.1.1";

        SocketAddress socketAddress{address};

        EXPECT_THAT(socketAddress.family(), Eq(AF_INET));
    }
    TEST(SocketAddressTestV4, SameAddressesAreEqual)
    {
        const std::string addressA = "192.168.0.1";
        const std::string addressB = "192.168.0.1";

        SocketAddress socketAddressA{addressA};
        SocketAddress socketAddressB{addressB};

        EXPECT_THAT(addressA, Eq(addressB));
    }
    TEST(SocketAddressTestV4, DifferentAddresesAreNotEqual)
    {
        const std::string addressA = "192.168.0.1";
        const std::string addressB = "192.168.0.0";

        SocketAddress socketAddressA{addressA};
        SocketAddress socketAddressB{addressB};

        EXPECT_THAT(addressA, Ne(addressB));
    }
    TEST(SocketAddressTestV6, AddressFamilyIsAF_INET6)
    {
        const std::string address = "2001:0db8:0000:0000:0000:0000:1428:57ab";

        SocketAddress socketAddress{address};

        EXPECT_THAT(socketAddress.family(), Eq(AF_INET6));
    }
    TEST(SocketAddressTestV6, SameAddressesAreEqual)
    {
        const std::string addressA = "2001:0db8:0000:0000:0000:0000:1428:57ab";
        const std::string addressB = "2001:0db8:0000:0000:0000:0000:1428:57ab";

        SocketAddress socketAddressA{addressA};
        SocketAddress socketAddressB{addressB};

        EXPECT_THAT(addressA, Eq(addressB));
    }
    
    TEST(SocketAddressTestV6, DifferentAddresesAreNotEqual)
    {
        const std::string addressA = "2001:0db8:0000:0000:0000:0000:1428:57ab";
        const std::string addressB = "FFFF:0db8:0000:0000:0000:0000:1428:57ab";

        SocketAddress socketAddressA{addressA};
        SocketAddress socketAddressB{addressB};

        EXPECT_THAT(addressA, Ne(addressB));
    }
    TEST(SocketAddressTestV6, StringIsRecognizedAsIPv4)
    {
        const std::string address = "2001:0db8:0000:0000:0000:0000:1428:57ab";

        SocketAddress socketAddress{address};

        EXPECT_THAT(socketAddress.isV6(), IsTrue);
    }
}