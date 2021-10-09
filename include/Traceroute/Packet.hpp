#pragma once
#include "HeaderTypes.hpp"
#include "SocketAddress.hpp"

namespace traceroute
{
    class Packet
    {
    public:
        Packet(const SocketAddress &source, const SocketAddress &destination);
        const SocketAddress &getSourceAddress() const;
        const SocketAddress &getDestinationAddress() const;
        int getFamily() const;
        
        virtual std::string serialize() const = 0;
        virtual ~Packet() {}
    protected:
        const SocketAddress mSourceAddress;
        const SocketAddress mDestinationAddress;
    };

   

   
    
}