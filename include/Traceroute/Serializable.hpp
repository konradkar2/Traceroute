#pragma once
#include <cstdint>
#include <cstring>
namespace Traceroute
{    
   
    template <typename POD>
    class  Serializer
    {
    public:
        static size_t Serialize_size(POD value)
        {
            return sizeof(POD);
        }
        static char* Serialize( char* target, POD value )
        {
            std::memcpy( target, &value, Serialize_size(value) );
            return target + Serialize_size(value);
        }
        static const char* Deserialize( const char* source, POD& target )
        {
            std::memcpy( &target, source, Serialize_size(target) );
            return source + Serialize_size(target);
        }
    };

    // template<>
    // size_t Serializer<char*>::Serialize_size(char* str)
    // {
    //     return sizeof(size_t) + strlen(str);
    // }

    // template<>
    // const char* Serializer<char*>::Deserialize( const char* source, char*& target )
    // {
    //     size_t length;
    //     std::memcpy( &length, source, sizeof(size_t) );
    //     std::memcpy( &target, source + sizeof(size_t), length );
    //     return source + sizeof(size_t) + length;
    // }
}
