#pragma once
#include <cstdint>
#include <cstring>
namespace Traceroute
{    
   
    template <typename POD>
    class  PodSerializer
    {
    public:
        static size_t getSize()
        {
            return sizeof(POD);
        }
        static char* serialize( char* target, POD value )
        {
            auto podSize = sizeof(value);
            std::memcpy( target, &value, podSize);
            return target + podSize;
        }
        static const char* deserialize( const char* source, POD& target )
        {
            auto podSize = sizeof(target);
            std::memcpy( &target, source, podSize );
            return source + podSize;
        }
    };

}
