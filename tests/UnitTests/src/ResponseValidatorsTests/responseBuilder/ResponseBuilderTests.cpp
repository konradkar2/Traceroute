#include "ResponseBuilder.hpp"
#include <gmock/gmock.h>

namespace traceroute
{
    class ResponseBuilderV4Tests : public testing::Test
    {
    public:
        ResponseBuilderV4 builder;
    };

    TEST_F(ResponseBuilderV4Tests, test)
    {
        const SocketAddress source;
        const SocketAddress destination;
        builder.buildIpHeader()
            .setSource(source)
            .setDestination(destination)
            .setIhl(4);
           
    }
}