#pragma once
#include <cstdint>
#include <cstring>
namespace Traceroute
{    
   
    template <typename POD>
    class  PodSerializer
    {
    public:
        static std::string serialize(POD pod)
        {
            auto podP = reinterpret_cast<const char*>(&pod);
            std::string serializedPod{podP,podP+sizeof(pod)};
            return serializedPod;
        }
    };

}
