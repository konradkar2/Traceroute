#include "IpHeaderBuilder.hpp"
#include <gmock/gmock.h>

namespace traceroute
{
    class IpHeaderBuilderTest : public testing::Test
    {
    public:
        IpHeaderBuilder builder;
    };

    TEST_F(IpHeaderBuilderTest, test)
    {
        const SocketAddress source;
        const SocketAddress destination;
        builder.setSource(source)
            .setDestination(destination)
            .setIhl(4);
        }
}