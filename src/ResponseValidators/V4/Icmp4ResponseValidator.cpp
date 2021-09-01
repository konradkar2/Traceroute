#include "Icmp4ResponseValidator.hpp"
#include <Traceroute/Packet/IcmpPacket.hpp>
#include <netinet/ip_icmp.h>
#include <cassert>
#include "Utils.hpp"
namespace traceroute::responseValidators::v4
{
    using namespace traceroute::packet;
    namespace
    {
        bool checkForEchoReply(const char *dataPtr, const IcmpPacket &icmpPacket, const SocketAddress &client);
        bool checkForTimeExceeded(const char *dataPtr, const IcmpPacket &icmpPacket);
    }

    bool Icmp4ResponseValidator::isResponseValid(const Packet &request, const SocketAddress &client,
                                                 int protocol, const char *response, size_t responseSize)
    {
        const auto &icmpPacket = dynamic_cast<const IcmpPacket &>(request);
        response = skipIpHeader(response);
        if (checkForEchoReply(response, icmpPacket, client) ||
            checkForTimeExceeded(response, icmpPacket))
            return true;

        return false;
    }

    namespace
    {
        bool checkForEchoReply(const char *icmpHdrP, const IcmpPacket &icmpPacket, const SocketAddress &client)
        {
            const IcmpHeader *icmpHdr = reinterpret_cast<const IcmpHeader *>(icmpHdrP);
            if (icmpHdr->type == ICMP_ECHOREPLY)
            {
                if (client == icmpPacket.getDestinationAddress() &&
                    icmpHdr->sequence == icmpPacket.GetIcmpHeader().sequence)
                    return true;
            }
            return false;
        }
        bool checkForTimeExceeded(const char *icmpHdrP, const IcmpPacket &icmpPacket)
        {
            const IcmpHeader *icmpHdr = reinterpret_cast<const IcmpHeader *>(icmpHdrP);
            if (icmpHdr->type == ICMP_TIME_EXCEEDED)
            {
                icmpHdrP += sizeof(IcmpHeader);
                icmpHdrP = skipIpHeader(icmpHdrP);
                const IcmpHeader *inner_icmp_hdr = reinterpret_cast<const IcmpHeader *>(icmpHdrP);
                if (inner_icmp_hdr->id == icmpPacket.GetIcmpHeader().id)
                    return true;
            }
            return false;
        }
    }

}
