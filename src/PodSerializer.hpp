#pragma once
#include <cstdint>
#include <string>
#include <type_traits>
#include <typeinfo>

namespace traceroute
{

    class PodSerializer
    {
    public:
        template <typename POD>
        static std::string serialize(const POD &pod)
        {
            static_assert(std::is_pod<POD>::value,"Not a pod");
            
            auto podP = reinterpret_cast<const char *>(&pod);
            return std::string{podP, podP + sizeof(pod)};
        }
    };

}
