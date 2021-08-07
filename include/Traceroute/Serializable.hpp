#pragma once
#include <cstdint>
#include <cstring>
namespace Traceroute
{    
   
    template <typename POD>
    class  Serializer
    {
    public:
        static size_t getSize(POD value)
        {
            return sizeof(POD);
        }
        static char* serialize( char* target, POD value )
        {
            std::memcpy( target, &value, getSize(value) );
            return target + getSize(value);
        }
        static const char* deserialize( const char* source, POD& target )
        {
            std::memcpy( &target, source, getSize(target) );
            return source + getSize(target);
        }
    };

}
