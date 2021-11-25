#pragma once
#include "HeaderTypes.hpp"
#include "SocketAddress.hpp"
#include "Traceroute/interface/IValidateResponse.hpp"
#include <memory>

namespace traceroute {
class Packet : public IValidateResponse
{
  public:
    Packet(const SocketAddress &source, const SocketAddress &destination);
    virtual ~Packet() = default;
    int                  getFamily() const;
    const SocketAddress &getSourceAddress() const;
    const SocketAddress &getDestinationAddress() const;
    virtual std::string  serialize() const = 0;

  protected:
    SocketAddress mSourceAddress;
    SocketAddress mDestinationAddress;
};

} // namespace traceroute