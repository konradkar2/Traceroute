#pragma once
#include <cstdint>
#include <cstring>
namespace traceroute
{

    class PodSerializer
    {
    public:
        template <typename POD>
        static std::string serialize(const POD &pod)
        {
            auto podP = reinterpret_cast<const char *>(&pod);
            return std::string{podP, podP + sizeof(pod)};
        }
    };

}
