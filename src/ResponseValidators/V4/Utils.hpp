#pragma once
#include <cstddef>

namespace traceroute::responseValidators::v4
{
size_t getIpHeaderSize(const char *ptr);
}
