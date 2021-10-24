#pragma once
#include <Traceroute/HeaderTypes.hpp>
#include <Traceroute/Packet.hpp>
#include <cassert>
#include <string>

namespace traceroute
{

template <int PaddingSize>
struct Ipv4HeaderCustomSize
{
    static_assert(PaddingSize % 4 == 0, "IpHeader size has to be multiplication of 4");
    Ipv4Header header;
    char padding [PaddingSize];
};


template<int PaddingSize>
Ipv4HeaderCustomSize<PaddingSize> createCustomSizeIpHeader()
{
    Ipv4HeaderCustomSize<PaddingSize> wrapper;
    wrapper.header.ihl = sizeof(wrapper) >> 2;
    wrapper.header.version = 4;
    return wrapper;
}

inline Ipv4Header createStandardIpHeader()
{
    Ipv4Header header;
    header.ihl = 5;
    header.version = 4;
    return header;
}


} // namespace traceroute