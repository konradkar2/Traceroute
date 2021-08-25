#pragma once
#include <cstdint>
#include <cstring>
namespace Traceroute
{

    class PodSerializer
    {
    public:
        template <typename POD>
        static std::string serialize(const POD &pod)
        {
            auto podP = reinterpret_cast<const char *>(&pod);
            std::string serializedPod{podP, podP + sizeof(pod)};
            return serializedPod;
        }
    };

}
